
#include "TcpSocket.h"

int main()
{

	CppServer::TcpSocket socket;

	if (!socket.IsInit())
		return 1;
    
	socket.Run();

	return 0;
}