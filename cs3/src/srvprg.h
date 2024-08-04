#ifndef SRVPRG_UTIL
#define SRVPRG_UTIL

#define _XOPEN_SOURCE \
  700  // Error in Visual Studio Code: incomplete type struct addrinfo is not
       // allowed

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>  // for fork
#endif                 // SRVPRG_UTIL

/* Functions */

SOCKET create_socket(const char *host, const char *port);
struct client_info *get_client(SOCKET s);
void drop_client(struct client_info *client);
const char *get_client_address(struct client_info *ci);
fd_set wait_on_clients(SOCKET server);
// void serve_resource (struct client_info *client, const char *path);
