#pragma once

#define WIN32_LEAN_AND_MEAN

#pragma comment (lib, "ws2_32.lib")

#include <iostream>
#include <WS2tcpip.h>

namespace XHost {
    void CleanWS()
    {
        WSACleanup();
    }

    void InitWS()
    {
        std::atexit(CleanWS);
        WSADATA data;
        WORD version = MAKEWORD(2, 2);
        int success = WSAStartup(version, &data);
        if (success != 0)
        {
            fprintf(stderr, "Failed: Can't initialize winsock!! %i\n", success);
        }
    }

    enum XSocketType
    {
        DEFAULT,
        NONE
    };
    typedef struct XClient
    {
        XClient(unsigned short port)
            :ws_socketLen(sizeof(ws_socket)), ws_socket()
        {
            ws_socket.sin_family = AF_INET;
            ws_socket.sin_port = htons(port);
        }
        void zero_buf()
        {
            memset(&ws_socket, 0, ws_socketLen);
        }
        int recv_data(SOCKET socket, char* buf, int len)
        {
            int temp = recv(socket, buf, len, 0);
            if (temp == SOCKET_ERROR)
            {
                fprintf(stderr, "ERROR: receiving from client! %i\n", WSAGetLastError());
            }
            return temp;
        }

        int ws_socketLen;
        sockaddr_in ws_socket;
    }XClient;

    typedef struct XSocket
    {
        SOCKET ws_socket;
        sockaddr_in ws_serverHint;

        XSocket(unsigned short port, int socket_type = SOCK_DGRAM, XSocketType type = DEFAULT)
            :ws_socket(socket(AF_INET, socket_type, 0))
        {
            if (type == NONE)
                return;

            if (ws_socket == INVALID_SOCKET)
            {
                fprintf(stderr, "Failed: Can't create socket!! %i\n", WSAGetLastError());
                exit(-1);
            }
            ws_serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
            ws_serverHint.sin_family = AF_INET;
            ws_serverHint.sin_port = htons(port);
        }

        void listener_socket()
        {
            listen(ws_socket, SOMAXCONN);
        }
        void socket_accept(XSocket socket, XClient client)
        {
            ws_socket = accept(socket.ws_socket, (sockaddr*)&client.ws_socket, &client.ws_socketLen);
        }
        void bind_socket()
        {
            int success = bind(ws_socket, (sockaddr*)&ws_serverHint, sizeof(ws_serverHint));
            if (success == SOCKET_ERROR)
            {
                fprintf(stderr, "Failed: Can't bind socket!! %i\n", WSAGetLastError());
                exit(-1);
            }
        }

        void connect_socket()
        {
            int success = connect(ws_socket, (sockaddr*)&ws_serverHint, sizeof(ws_serverHint));
            if (success == SOCKET_ERROR)
            {
                fprintf(stderr, "Failed: Can't connect to server!! %i\n", WSAGetLastError());
                exit(-1);
            }
        }

        int recv_data(char* buf, int len)
        {
            int bytesReceived = recv(ws_socket, buf, len, 0);
            if (bytesReceived == SOCKET_ERROR)
            {
                fprintf(stderr, "ERROR: While receiving data! %i\n", WSAGetLastError());
                exit(-1);
            }
            return bytesReceived;
        }

        void send_data(char* buf, int len)
        {
            int success = send(ws_socket, buf, len, 0);
            if (success == SOCKET_ERROR)
            {
                fprintf(stderr, "Failed: Couldn't send data!! %i\n", WSAGetLastError());
                exit(-1);
            }
        }

        void set_ip(char* address)
        {
            int success = inet_pton(AF_INET, address, &ws_serverHint.sin_addr);
            if (success != 1)
            {
                fprintf(stderr, "Failed: Couldn't set IP!! %i\n", WSAGetLastError());
            }
        }

        void close_socket()
        {
            closesocket(ws_socket);
        }
    }XSocket;
}