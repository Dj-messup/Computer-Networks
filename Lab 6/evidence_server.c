#define _POSIX_C_SOURCE 200112L

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_PORT "3490"
#define BACKLOG 10
#define BUFFER_SIZE 64
#define MAX_MESSAGE 100
#define CASE_FILE "case_file.txt"

static int send_all(int socket_fd, const void *buffer, size_t length)
{
    const char *data = buffer;
    size_t total_sent = 0;

    while (total_sent < length) {
        ssize_t sent = send(socket_fd, data + total_sent, length - total_sent, 0);

        if (sent == -1) {
            perror("send");
            return -1;
        }

        total_sent += (size_t)sent;
    }

    return 0;
}

static int create_server_socket(const char *port)
{
    struct addrinfo hints;
    struct addrinfo *server_info;
    struct addrinfo *current;
    int socket_fd = -1;
    int yes = 1;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, port, &hints, &server_info);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }

    for (current = server_info; current != NULL; current = current->ai_next) {
        socket_fd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);

        if (socket_fd == -1) {
            continue;
        }

        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("setsockopt");
            close(socket_fd);
            freeaddrinfo(server_info);
            return -1;
        }

        if (bind(socket_fd, current->ai_addr, current->ai_addrlen) == 0) {
            break;
        }

        close(socket_fd);
        socket_fd = -1;
    }

    freeaddrinfo(server_info);

    if (socket_fd == -1) {
        fprintf(stderr, "Evidence server failed to bind.\n");
        return -1;
    }

    if (listen(socket_fd, BACKLOG) == -1) {
        perror("listen");
        close(socket_fd);
        return -1;
    }

    return socket_fd;
}

static void run_echo_mode(int client_fd)
{
    char message[MAX_MESSAGE + 1];

    while (1) {
        ssize_t bytes_received = recv(client_fd, message, MAX_MESSAGE, 0);

        if (bytes_received == -1) {
            perror("recv");
            break;
        }

        if (bytes_received == 0) {
            printf("Investigator client disconnected.\n");
            break;
        }

        message[bytes_received] = '\0';

        printf("Evidence note received: %s\n", message);

        if (strcmp(message, "close") == 0) {
            const char goodbye[] = "goodbye";
            send_all(client_fd, goodbye, strlen(goodbye));
            break;
        }

        send_all(client_fd, message, strlen(message));
    }
}

static void run_file_transfer_mode(int client_fd)
{
    FILE *file = fopen(CASE_FILE, "rb");
    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    if (file == NULL) {
        perror("fopen");
        return;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send_all(client_fd, buffer, bytes_read) == -1) {
            break;
        }
    }

    if (ferror(file)) {
        perror("fread");
    }

    fclose(file);

    printf("Case file transfer complete.\n");
}

static void handle_client(int client_fd)
{
    char mode[10];
    ssize_t bytes_received = recv(client_fd, mode, sizeof(mode) - 1, 0);

    if (bytes_received <= 0) {
        if (bytes_received == -1) {
            perror("recv mode");
        }
        return;
    }

    mode[bytes_received] = '\0';

    printf("Mode selected: %s\n", mode);

    if (strcmp(mode, "1") == 0) {
        printf("Entering evidence note echo mode.\n");
        run_echo_mode(client_fd);
    } else if (strcmp(mode, "2") == 0) {
        printf("Entering case file transfer mode.\n");
        run_file_transfer_mode(client_fd);
    } else {
        const char error[] = "Invalid mode. Use 1 for echo or 2 for file transfer.";
        send_all(client_fd, error, strlen(error));
    }
}

int main(int argc, char *argv[])
{
    const char *port = DEFAULT_PORT;
    int server_fd;

    if (argc == 2) {
        port = argv[1];
    } else if (argc > 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        return 1;
    }

    server_fd = create_server_socket(port);

    if (server_fd == -1) {
        return 1;
    }

    printf("Cyber Forensics Evidence Vault listening on port %s...\n", port);

    while (1) {
        struct sockaddr_storage client_address;
        socklen_t address_size = sizeof(client_address);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &address_size);

        if (client_fd == -1) {
            perror("accept");
            continue;
        }

        printf("Investigator client connected.\n");

        handle_client(client_fd);

        close(client_fd);

        printf("Investigator client connection closed.\n");
    }

    close(server_fd);

    return 0;
}
