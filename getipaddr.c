#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static void print_address(const struct addrinfo *entry)
{
    char ip_string[INET6_ADDRSTRLEN];
    void *address_pointer = NULL;
    const char *version = NULL;

    if (entry->ai_family == AF_INET) {
        struct sockaddr_in *ipv4_address = (struct sockaddr_in *)entry->ai_addr;
        address_pointer = &(ipv4_address->sin_addr);
        version = "IPv4";
    } else if (entry->ai_family == AF_INET6) {
        struct sockaddr_in6 *ipv6_address = (struct sockaddr_in6 *)entry->ai_addr;
        address_pointer = &(ipv6_address->sin6_addr);
        version = "IPv6";
    } else {
        return;
    }

    if (inet_ntop(entry->ai_family, address_pointer, ip_string, sizeof(ip_string)) == NULL) {
        perror("inet_ntop");
        return;
    }

    printf("  %-4s %s\n", version, ip_string);
}

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *address_list = NULL;
    struct addrinfo *current = NULL;
    int status;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        fprintf(stderr, "Example: %s example.com\n", argv[0]);
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(argv[1], NULL, &hints, &address_list);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return 2;
    }

    printf("IP addresses for %s:\n", argv[1]);

    for (current = address_list; current != NULL; current = current->ai_next) {
        print_address(current);
    }

    freeaddrinfo(address_list);

    return 0;
}
