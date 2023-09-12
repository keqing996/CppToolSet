#include "../../include/WindowsApi.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

namespace WindowsApi
{
    std::wstring WinGetUserName()
    {
        wchar_t userNameBuffer[255];
        DWORD userNameBufferLen;
        ::GetUserNameW(userNameBuffer, &userNameBufferLen);
        return { userNameBuffer };
    }

    bool WinCreateProcess(std::wstring cmd)
    {
        STARTUPINFO si;
        ::ZeroMemory( &si, sizeof(si) );
        si.cb = sizeof(si);

        PROCESS_INFORMATION pi;
        ::ZeroMemory( &pi, sizeof(pi) );

        // Start the child process.
        if( !::CreateProcessW(
                nullptr,                                // No module name (use command line)
                const_cast<LPWSTR>(cmd.c_str()),        // Command line
                nullptr,                                // Process handle not inheritable
                nullptr,                                // Thread handle not inheritable
                FALSE,                                  // Set handle inheritance to FALSE
                0,                                      // No creation flags
                nullptr,                                // Use parent's environment block
                nullptr,                                // Use parent's starting directory
                &si,                                    // Pointer to STARTUPINFO structure
                &pi )                                   // Pointer to PROCESS_INFORMATION structure
                )
        {
            return false;
        }

        // Wait until child process exits.
        ::WaitForSingleObject( pi.hProcess, INFINITE );

        // Close process and thread handles.
        ::CloseHandle( pi.hProcess );
        ::CloseHandle( pi.hThread );

        return true;
    }

    bool Socket::WinWSAStartUp()
    {
        WORD wVersion = MAKEWORD(2, 2);
        WSADATA wsadata;

        if (0 != ::WSAStartup(wVersion, &wsadata))
        {
            return false;
        }

        if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2)
        {
            ::WSACleanup();
            return false;
        }

        return true;
    }

    void Socket::WinWSACleanUp()
    {
        ::WSACleanup();
    }
}
