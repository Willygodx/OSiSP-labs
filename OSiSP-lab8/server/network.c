#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_server(const char *port, struct addrinfo **result) {
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int soc = getaddrinfo(NULL, port, &hints, result);

    if (soc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(soc));
        exit(EXIT_FAILURE);
    }
}
