#include "network.h"
#include "processing.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo* result = get_server_info(argv[1]);
    int fd_client = connect_to_server(result);

    read(fd_client, BUFFER, MAX_LEN_BUFFER);
    printf("%s", BUFFER);

    bool flag_end = false;
    while (flag_end == false) {
        printf("> ");
        fgets(QUERY, MAX_LEN_BUFFER, stdin);
        if (QUERY[0] == '@') {
            QUERY[strlen(QUERY) - 1] = '\0';
            processing_query_from_file(fd_client, &flag_end);
        } else {
            proccesing_query(fd_client, &flag_end);
        }
    }

    close(fd_client);
    return 0;
}
