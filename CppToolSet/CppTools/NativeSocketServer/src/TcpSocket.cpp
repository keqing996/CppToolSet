

#include <iostream>
#include <vector>

#include "TcpSocket.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

// netstat -ano
// 查看占用端口
#define DEFAULT_PORT 4869

namespace CppServer
{
    TcpSocket::TcpSocket()
    {
    	std::string defaultIp = "0.0.0.0";
    	auto hostIpList = GetHostIp();
    	if (!hostIpList.empty())
    	{
    		defaultIp = hostIpList[0];
    		std::cout << "host ip: " << hostIpList[0] << std::endl;
    	}
        else
        {
        	std::cout << "host ip not found" << std::endl;
        }
    	
        _init = false;
        
        WORD wVersion = MAKEWORD(2, 2);
        WSADATA wsadata;
        if (0 != WSAStartup(wVersion, &wsadata))
        {
            std::cout << "socket init failed" << std::endl;
            return;
        }

        if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
        {
            std::cout << "socket version failed" << std::endl;
            WSACleanup();  
            return;
        }

        _serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    	IN_ADDR targetIp;
    	InetPtonA(AF_INET, defaultIp.c_str(), &targetIp);

        SOCKADDR_IN server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr = targetIp;
        server_addr.sin_port = htons(DEFAULT_PORT);

        if (bind(_serverSocket, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            std::cout << "socket bind failed" << std::endl;
            WSACleanup();
            return;
        }

        if (listen(_serverSocket, SOMAXCONN) < 0)
        {
            std::cout << "socket listen failed" << std::endl;
            WSACleanup();
            return;
        }

        std::cout << "socket init success" << std::endl;
        _init = true;
    }

    TcpSocket::~TcpSocket()
    {
        closesocket(_serverSocket);
        WSACleanup();
    }

    void TcpSocket::Run()
    {
        while (true)
        {
            std::cout << "run.." << std::endl;

            int len = sizeof(SOCKADDR);
            SOCKADDR_IN accept_addr;
            SOCKET clientSocket = accept(_serverSocket, reinterpret_cast<SOCKADDR*>(&accept_addr), &len);
            if (clientSocket == SOCKET_ERROR)
            {
                std::cout << "socket error.." << std::endl;
                break;
            }

            std::cout << "accept.." << std::endl;

        	char szRecvBuffer[1500];
        	
            while (true)
            {
                ZeroMemory(szRecvBuffer, sizeof(szRecvBuffer));

                int result = recv(clientSocket, szRecvBuffer, sizeof(szRecvBuffer), 0);
                if (0 == result) // client over
                {
                    break;
                }

                if (SOCKET_ERROR == result)
                {
                    std::cout << "receive fail " << WSAGetLastError() << std::endl;
                    break;
                }

                std::cout << "receive: " << szRecvBuffer << std::endl;
            }
        }
    }

    bool TcpSocket::IsInit() const
    {
        return _init;
    }

    std::vector<std::string> TcpSocket::GetHostIp() const
    {
        std::vector<std::string> ip;
    	
		// PIP_ADAPTER_INFO结构体指针存储本机网卡信息
		PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
    	
		unsigned long stSize = sizeof(IP_ADAPTER_INFO);
		int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    	
    	if (ERROR_BUFFER_OVERFLOW == nRel)
    	{
		    // 如果函数返回的是ERROR_BUFFER_OVERFLOW
    		// 则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
    		// 这也是说明为什么stSize既是一个输入量也是一个输出量
    		// 释放原来的内存空间
    		delete pIpAdapterInfo;
    		// 重新申请内存空间用来存储所有网卡信息
    		pIpAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(new BYTE[stSize]);
    		// 再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
    		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    	}
    	
		if (ERROR_SUCCESS == nRel)
		{
			while (pIpAdapterInfo)
			{
				// std::cout << "Net Card Desc: " << pIpAdapterInfo->Description << std::endl;
				switch (pIpAdapterInfo->Type)
				{
				case MIB_IF_TYPE_ETHERNET:
				default://无线网卡在这里,Unknown type
					{
						// 可能网卡有多IP,因此通过循环去判断
						IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
						do
						{
							if (strcmp(pIpAddrString->IpAddress.String, "0.0.0.0") != 0)
							{
								ip.emplace_back(pIpAddrString->IpAddress.String);
							}
							
							//"子网地址："pIpAddrString->IpMask.String
							//"网关地址："pIpAdapterInfo->GatewayList.IpAddress.String
							pIpAddrString = pIpAddrString->Next;
						} while (pIpAddrString);
					}
					break;
				case MIB_IF_TYPE_TOKENRING:
				case MIB_IF_TYPE_OTHER:
				case MIB_IF_TYPE_FDDI:
				case MIB_IF_TYPE_PPP:
				case MIB_IF_TYPE_LOOPBACK:
				case MIB_IF_TYPE_SLIP:
					break;
				}
				/* 网卡MAC地址： pIpAdapterInfo->Address[i] */
				pIpAdapterInfo = pIpAdapterInfo->Next;
			}
 	
		}

    	delete pIpAdapterInfo;

    	return ip;
    }
}
