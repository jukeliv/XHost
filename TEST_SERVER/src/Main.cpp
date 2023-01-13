#include "XHost.h"
using namespace XHost;

int main()
{
	InitWS();
	XSocket socket(8008, SOCK_STREAM);
	socket.bind_socket();

	socket.set_ip((char*)"127.0.0.1");

	listen(socket.ws_socket, SOMAXCONN);

	XClient client(8008);
	XSocket clientSocket(0, 0, NONE);
	clientSocket.socket_accept(socket, client);
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	memset(host, 0, NI_MAXHOST);
	memset(service, 0, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client.ws_socket, client.ws_socketLen, host, NI_MAXHOST, service, NI_MAXSERV, 0) != 0)
	{
		inet_ntop(AF_INET, &client.ws_socket.sin_addr, host, NI_MAXHOST);
		printf("%s connected on port %i\n", host, ntohs(client.ws_socket.sin_port));
	}

	printf("%s connected on port %s\n", host, service);

	socket.close_socket();

	char buf[4096];
	do {
		memset(buf, 0, 4096);
		int bytesReceived = clientSocket.recv_data(buf, 4096);
		if (bytesReceived == 0) {
			printf("%s client disconnected\n", host);
		}

		clientSocket.send_data(buf, bytesReceived + 1);
	} while (true);
	
	clientSocket.close_socket();

	return EXIT_SUCCESS;
}