#include "network.h"

struct addrinfo* get_server_info(const char* port) {
    struct addrinfo hints = {0};
    struct addrinfo* result = NULL;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;

    int s = getaddrinfo(NULL, port, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    return result;
}

int connect_to_server(struct addrinfo* result) {
    int fd_client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (fd_client == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int flag_connect = connect(fd_client, result->ai_addr, result->ai_addrlen);
    if (flag_connect == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    return fd_client;
}
