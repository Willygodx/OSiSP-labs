#ifndef OSISP_LAB8_UTILS_H
#define OSISP_LAB8_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LEN_BUFFER 1024
#define QUIT_QUERY "QUIT"

extern char BUFFER[MAX_LEN_BUFFER];
extern char CORRECT_BUFFER[MAX_LEN_BUFFER];
extern char QUERY[MAX_LEN_BUFFER];

bool is_end_session();

#endif //OSISP_LAB8_UTILS_H
