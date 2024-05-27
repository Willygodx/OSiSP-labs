#include "processing.h"
#include "utils.h"
#include <sys/types.h>
#include <sys/socket.h>

void proccesing_query(int __fd_client, bool* flag_end) {
    write(__fd_client, QUERY, MAX_LEN_BUFFER);
    *flag_end = is_end_session();
    ssize_t bytes_read = recv(__fd_client, QUERY, MAX_LEN_BUFFER, 0);
    QUERY[bytes_read] = '\0';
    if (bytes_read > 0) {
        fputs(QUERY, stdout);
    } else {
        fprintf(stderr, "The server dropped the connection\n");
        close(__fd_client);
        exit(EXIT_FAILURE);
    }
}

void processing_query_from_file(int __fd_client, bool* flag_end) {
    FILE* file = fopen(QUERY + 1, "r");
    if (file == NULL) {
        printf("File is not found.\n");
        return;
    }
    while (!feof(file)) {
        fgets(QUERY, MAX_LEN_BUFFER, file);
        printf("> %s", QUERY);
        proccesing_query(__fd_client, flag_end);
        if (*flag_end == true) {
            return;
        }
    }
}
