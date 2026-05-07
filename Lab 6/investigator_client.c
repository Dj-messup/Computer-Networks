#define _POSIX_C_SOURCE 200112L

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 64
#define MAX_MESSAGE 100
#define RECEIVED_FILE "received_case_file.txt"

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

static int connect_to_server(const char *hostname, const char *port)
{
    struct addrinfo hints;
    struct addrinfo *server_info;
    struct addrinfo *current;
    int socket_fd = -1;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &hints, &server_info);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return -1;
    }

    for (current = server_info; current != NULL; current = current->ai_next) {
        socket_fd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);

        if (socket_fd == -1) {
            continue;
        }

        if (connect(socket_fd, current->ai_addr, current->ai_addrlen) == 0) {
            break;
        }

        close(socket_fd);
        socket_fd = -1;
    }

    freeaddrinfo(server_info);

    if (socket_fd == -1) {
        fprintf(stderr, "Investigator client failed to connect.\n");
    }

    return socket_fd;
}

static void run_echo_mode(int socket_fd)
{
    char input[MAX_MESSAGE + 1];
    char response[MAX_MESSAGE + 1];

    while (1) {
        printf("Enter evidence note, or type close: ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nInput closed.\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            printf("Please enter at least one character.\n");
            continue;
        }

        if (send_all(socket_fd, input, strlen(input)) == -1) {
            break;
        }

        ssize_t bytes_received = recv(socket_fd, response, MAX_MESSAGE, 0);

        if (bytes_received == -1) {
            perror("recv");
            break;
        }

        if (bytes_received == 0) {
            printf("Evidence server closed the connection.\n");
            break;
        }

        response[bytes_received] = '\0';

        printf("Evidence server replied: %s\n", response);

        if (strcmp(input, "close") == 0 || strcmp(response, "goodbye") == 0) {
            break;
        }
    }
}

static void run_file_transfer_mode(int socket_fd)
{
    FILE *file = fopen(RECEIVED_FILE, "wb");
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    if (file == NULL) {
        perror("fopen");
        return;
    }

    while ((bytes_received = recv(socket_fd, buffer, sizeof(buffer), 0)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, (size_t)bytes_received, file);

        if (bytes_written != (size_t)bytes_received) {
            perror("fwrite");
            break;
        }
    }

    if (bytes_received == -1) {
        perror("recv");
    }

    fclose(file);

    printf("Case file received and saved as %s\n", RECEIVED_FILE);
}

int main(int argc, char *argv[])
{
    const char *hostname;
    const char *port;
    const char *mode_argument;
    const char *mode_to_send;
    int socket_fd;

    if (argc != 4) {
        fprintf(stderr, "Usage: %s hostname port echo|file\n", argv[0]);
        fprintf(stderr, "Example echo: %s localhost 3490 echo\n", argv[0]);
        fprintf(stderr, "Example file: %s localhost 3490 file\n", argv[0]);
        return 1;
    }

    hostname = argv[1];
    port = argv[2];
    mode_argument = argv[3];

    if (strcmp(mode_argument, "echo") == 0 || strcmp(mode_argument, "1") == 0) {
        mode_to_send = "1";
    } else if (strcmp(mode_argument, "file") == 0 || strcmp(mode_argument, "2") == 0) {
        mode_to_send = "2";
    } else {
        fprintf(stderr, "Mode must be echo, file, 1, or 2.\n");
        return 1;
    }

    socket_fd = connect_to_server(hostname, port);

    if (socket_fd == -1) {
        return 1;
    }

    if (send_all(socket_fd, mode_to_send, strlen(mode_to_send)) == -1) {
        close(socket_fd);
        return 1;
    }

    if (strcmp(mode_to_send, "1") == 0) {
        run_echo_mode(socket_fd);
    } else {
        run_file_transfer_mode(socket_fd);
    }

    close(socket_fd);

    return 0;
}
