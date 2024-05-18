/* web_get.c */

#include "code06.h"

void parse_url(char *url, char **hostname, char **port, char **path);

void parse_url(char *url, char **hostname, char **port, char **path)
{
    printf("URL: %s\n", url);
    char *p;
    p = strstr(url, "://");
    char *protocol = 0;
    // p points to occourence of search string
    if (p)
    {
        protocol = url;
        *p = 0;
        p += 3;
    }
    else
    {
        p = url;
    }
    if (protocol)
    {
        if (strcmp(protocol, "http"))
        {
            fprintf(stderr, "Unknown protocol '%s'. Only 'http' is supported.\n", protocol);
            exit(1);
        }
    }
    // hostname
    *hostname = p;
    while (*p && *p != ':' && *p != '/' && *p != '#')
        ++p;
    // check for possible port number in url
    *port = 80;
    if (*p == ':')
    {
        *p++;
        *port = p;
    }
    while (*p && *p != '/' && *p != '#')
        ++p;

    *path = p;
    if (*p == '/')
    {
        *path = p + 1;
    }
    *p = 0;

    // check for #
    while (*p && *p != '#')
        ++p;
    if (*p == '#')
        *p = 0;
    printf("hostname: %s\n", *hostname);
    printf("port: %s\n", *port);
    printf("path: %s\n", *path);
}