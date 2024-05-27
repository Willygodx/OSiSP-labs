#ifndef OSISP_LAB8_NETWORK_H
#define OSISP_LAB8_NETWORK_H

#include <netdb.h>

void init_server(const char *port, struct addrinfo **result);

#endif //OSISP_LAB8_NETWORK_H
