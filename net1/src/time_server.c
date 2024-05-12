/* timeserver */
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
#include <sys/socket.h>
#include <sys/types.h>
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
#define GETSOCKETERRNO() (errno)
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>


int main(void) {
#if defined(_WIN32)
  WSADATA d;
  if (WSAStartup(MAKEWORD(2, 2), &d)) {
    fprintf(stderr, "Failed to initialize.\n");
    return 1;
  }
#endif
  printf("Configuration of local address ...\n");
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *bind_address;
  getaddrinfo(0, "8080", &hints, &bind_address);

  printf("\ncreate socket ...\n");
  int socket_listen;
  socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if (!ISVALIDSOCKET(socket_listen)) {
    fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
  }

  int option = 0;
  if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, sizeof(option))) {
    fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
  }

  printf("Binding socket to local address ...\n");
  if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
    fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
  }
  freeaddrinfo(bind_address);

  printf("Listening ...\n");
  if(listen(socket_listen,10) < 0) {  // die 10 ist die Anzahl der Anfragen die gequeued werden
    fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
  }

  printf("Waiting for connection ...\n");
  struct sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);
  int socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);
  if(!ISVALIDSOCKET(socket_client)){
    fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
  }

  printf("Client is connected ...");
  char address_buffer[100];
  getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), 0,0, NI_NUMERICHOST);
  printf("%s\n", address_buffer);

  printf("Reading request ...\n");
  char request[1024];
  ssize_t bytes_received = recv(socket_client, request, 1024, 0);
  // hier wäre noch eine Fehlerbehandlung notwendig
  printf("Received %ld bytes.\n", bytes_received);
  
  printf("Sending response...\n");
  const char *response = 
     "HTTP/1.1 200 OK\r\n"
     "Connection: close\r\n"
     "Content-Type: text/plain\r\n\r\n"
     "Local time is: ";
  ssize_t bytes_sent = send(socket_client, response, strlen(response), 0);
  printf("Sent %ld of %d bytes.\n", bytes_sent, (int)strlen(response));

  time_t timer;
  time(&timer);
  char *time_msg = ctime(&timer);
  bytes_sent = send(socket_client, time_msg, strlen(time_msg),0);
  printf("Sent %ld of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

  printf("Closing connection ...\n");
  CLOSESOCKET(socket_client);

  #if defined(_WIN32)
    WSACleanup();
  #endif

  printf("Finished.\n");
  return 0;
}
