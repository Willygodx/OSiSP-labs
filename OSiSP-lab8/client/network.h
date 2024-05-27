#ifndef OSISP_LAB8_NETWORK_H
#define OSISP_LAB8_NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

struct addrinfo* get_server_info(const char* port);
int connect_to_server(struct addrinfo* result);

#endif //OSISP_LAB8_NETWORK_H
