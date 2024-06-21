/* udp_recvfrom.c*/
#include "udp04.h"

int main(void)
{
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints)); // möglliche Reste löschen
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM; // UDP
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(
        bind_address->ai_family,
        bind_address->ai_socktype,
        bind_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_listen))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

    printf("Binding socket to local address....\n");
    if (bind(socket_listen,
             bind_address->ai_addr,
             bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }
    freeaddrinfo(bind_address);

    // ab hier unterschied zu tcp code
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    char read[1024];
    ssize_t bytes_received = recvfrom(socket_listen,
                                      read,
                                      1024,
                                      0,
                                      (struct sockaddr *)&client_address,
                                      &client_len);

    printf("Received (%ld bytes): %.*s\n", bytes_received, (int)bytes_received, read);

    printf("Remote address is:");
    char address_buffer[100];
    char service_buffer[100];
    getnameinfo(((struct sockaddr *)&client_address),
                client_len,
                address_buffer, sizeof(address_buffer),
                service_buffer, sizeof(service_buffer),
                NI_NUMERICHOST | NI_NUMERICSERV);
    printf("%s, %s\n", address_buffer, service_buffer);

    CLOSESOCKET(socket_listen);
#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");
    return 0;
}