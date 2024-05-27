#include "server.h"
#include "utils.h"
#include "file.h"
#include "logger.h"
#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

char QUERY[MAX_LEN_BUFFER];
bool is_new_dir = false;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Incorrect arguments!\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    strncpy(CURRENT_DIR, ".", strlen("."));
    DIR *d = opendir(argv[2]);
    if (d == NULL) {
        printf("The current directory could not be opened. "
               "Please check that the data is correct.");
        exit(EXIT_FAILURE);
    }

    getcwd(CURRENT_DIR, MAX_LEN_BUFFER);
    struct addrinfo *result = NULL;
    init_server(argv[1], &result);

    int socfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    bind(socfd, result->ai_addr, result->ai_addrlen);

    printf("myserver %s\n", argv[1]);
    printf("Ready.\n");

    listen(socfd, SOMAXCONN);
    int fd_client = accept(socfd, result->ai_addr, &result->ai_addrlen);

    execute_dir_args(d);

    const char *filename = argv[3];
    char *serverinfo = NULL;
    size_t file_size = 0;
    execute_file_args(filename, &serverinfo, &file_size);
    write(fd_client, serverinfo, file_size);

    while (true) {
        ssize_t bytes_read = recv(fd_client, QUERY, MAX_LEN_BUFFER, 0);
        size_t size_query = strlen(QUERY);
        if (bytes_read > 0) {
            if (strncasecmp(QUERY, ECHO_QUERY, strlen(ECHO_QUERY)) == 0) {
                logging_console(fd_client, ECHO_QUERY, QUERY, __DEBUG);
                write(fd_client, QUERY + strlen(ECHO_QUERY) + 1, size_query);
            } else if (strncasecmp(QUERY, INFO_QUERY, strlen(INFO_QUERY)) == 0) {
                logging_console(fd_client, INFO_QUERY, QUERY, __DEBUG);
                write(fd_client, serverinfo, file_size);
            } else if (strncasecmp(QUERY, CD_QUERY, strlen(CD_QUERY)) == 0) {
                logging_console(fd_client, CD_QUERY, QUERY, __DEBUG);
                QUERY[size_query - 1] = '\0';
                if ((d = opendir(QUERY + 3)) == NULL) {
                    write(fd_client, "", 1);
                } else {
                    strncpy(CURRENT_DIR, QUERY + 3, size_query);
                    chdir(CURRENT_DIR);
                    getcwd(CURRENT_DIR, MAX_LEN_BUFFER);
                    d = opendir(CURRENT_DIR);
                    QUERY[size_query - 1] = '\n';
                    write(fd_client, QUERY + 3, size_query - 3);
                    is_new_dir = true;
                }
            } else if (strncasecmp(QUERY, LIST_QUERY, strlen(LIST_QUERY)) == 0) {
                logging_console(fd_client, LIST_QUERY, QUERY, __DEBUG);
                if (is_new_dir == true) {
                    execute_dir_args(d);
                    is_new_dir = false;
                }
                write(fd_client, LIST_DIR, strlen(LIST_DIR));
            } else if (strncasecmp(QUERY, QUIT_QUERY, strlen(QUIT_QUERY)) == 0) {
                logging_console(fd_client, QUIT_QUERY, QUERY, __DEBUG);
                write(fd_client, EXIT_MESSAGE, strlen(EXIT_MESSAGE));
            } else {
                logging_console(fd_client, UNKNOWN_QUERY, QUERY, __ERROR);
                write(fd_client, WRONG_PARAMETER, strlen(WRONG_PARAMETER));
            }
        } else if (bytes_read == 0) {
            printf("Client is closed connection\n");
            break;
        } else {
            fprintf(stderr, "Error of reading data.\n");
        }
    }

    free(serverinfo);

    close(fd_client);
    close(socfd);

    return 0;
}
