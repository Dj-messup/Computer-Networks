#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *p;
    int status;
    char ip[INET6_ADDRSTRLEN];

    if (argc != 2) {
        printf("Usage: %s hostname\n", argv[0]);
        printf("Example: %s example.com\n", argv[0]);
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(argv[1], NULL, &hints, &result);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n", argv[1]);

    for (p = result; p != NULL; p = p->ai_next) {
        void *addr;
        char *type;
        int family = p->ai_family;

        if (family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            type = "IPv4";
        } else if (family == AF_INET6) {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            type = "IPv6";
        } else {
            continue;
        }

        if (inet_ntop(family, addr, ip, sizeof ip) == NULL) {
            perror("inet_ntop");
            continue;
        }

        printf("  %s: %s\n", type, ip);
    }

    freeaddrinfo(result);

    return 0;
}
