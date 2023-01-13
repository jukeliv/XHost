#include "XHost.h"
#include <string>

using namespace XHost;

int main()
{
	std::string ipAddress = "127.0.0.1";				// IP Address of the server
	unsigned short port = 8008;						// Listening port # on the server

	InitWS();

	XSocket socket(port, SOCK_STREAM);
	socket.set_ip((char *)ipAddress.c_str());
	socket.connect_socket();
	char buf[4096];

	socket.send_data((char*)"hello world", strlen("hello world") + 1);

	memset(buf, 0, 4096);
	int bytesReceived = socket.recv_data(buf, 4096);
	if (bytesReceived > 0)
	{
		printf("SERVER> %s\n", std::string(buf, 0, bytesReceived).c_str());
	}

	return EXIT_SUCCESS;
}