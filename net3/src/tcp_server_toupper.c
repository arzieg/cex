/* tcp_server_toupper.c */
#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#endif

// IPv6 Support fuer Windows Vista
#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif

// for _kbit()
#if defined(_WIN32)
#include <conio.h>
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    #if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2,2), &d)) {
        fprintf (stderr, "Failed to initialize.\n");
        return 1;
    }
    #endif

    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address..\n");
    if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;

    }
    freeaddrinfo(bind_address);
    printf("Listening...\n");
    if(listen(socket_listen, 10) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    // fd_set stores all of the active sockets
    // max_socket hold the largest socket descriptor
    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    SOCKET max_socket = socket_listen;

    printf("Waiting for connection...\n");
    while(1){
        fd_set reads;
        reads = master;
        if(select(max_socket+1, &reads, 0,0,0) < 0) {
            fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
            return 1;
        }

        SOCKET i;
        for(i=1; i<=max_socket; ++i){
            if(FD_ISSET(i,&reads)){
                // Handle socket
                if (i == socket_listen) {
                    struct sockaddr_storage client_address;
                    socklen_t client_len = sizeof(client_address);
                    SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);
                    if (!ISVALIDSOCKET(socket_client)){
                        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
                        return 1;
                    }
                    FD_SET(socket_client, &master);
                    if(socket_client > max_socket)
                      max_socket = socket_client;

                    char address_buffer[100];
                    getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), 0,0, NI_NUMERICHOST);
                    printf("New connection from %s\n", address_buffer);
                } else {
                    char read[1024];
                    ssize_t bytes_received = recv(i, read, 1024, 0);
                    if(bytes_received< 1) {
                        FD_CLR(i, &master);
                        CLOSESOCKET(i);
                        continue;
                    }
                    SOCKET j;
                    for (j=1; j<=max_socket; ++j) {
                        if(FD_ISSET(j, &master)) {
                            if (j == socket_listen || j==i)
                              continue;
                            else
                              send(j, read, (size_t)bytes_received, 0);
                        }
                    }
                    /* alter code 
                    int j;
                    for (j=0; j<bytes_received; ++j)
                      read[j] = (char)toupper(read[j]);
                    send(i, read, (size_t)bytes_received,0);
                    */
                }
            } // if FD_ISSET
        } // for i to max_socket
    } // while(1)

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);

    #if defined(_WIN32)
      WSACleanup();
    #endif

      printf("Finished.\n");
      return 0;
}