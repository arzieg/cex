// client
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("\nSocket creation error \n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // convert ipv5 and ipv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("\nInvalid address. Address not supported \n");
        exit(EXIT_FAILURE);
    }
    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        perror("\nConnection failed\n");
        exit(EXIT_FAILURE);
    }
    send(client_fd, hello, strlen(hello), 0);
    printf("\nHello message sent\n");
    valread = read(client_fd, buffer, 1024 - 1); // -1 for null terminator
    printf("%s\n", buffer);

    close(client_fd);
    return 0;
}