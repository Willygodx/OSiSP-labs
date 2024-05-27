#ifndef OSISP_LAB8_PROCESSING_H
#define OSISP_LAB8_PROCESSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_LEN_BUFFER 1024
extern char QUERY[MAX_LEN_BUFFER];

void proccesing_query(int __fd_client, bool* flag_end);
void processing_query_from_file(int __fd_client, bool* flag_end);

#endif //OSISP_LAB8_PROCESSING_H
