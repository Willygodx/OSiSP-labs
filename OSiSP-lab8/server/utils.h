#ifndef OSISP_LAB8_UTILS_H
#define OSISP_LAB8_UTILS_H

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_LEN_BUFFER 1024
#define LINK_TO_FILE " --> "
#define LINK_TO_LINK " -->> "

extern char CURRENT_DIR[MAX_LEN_BUFFER];
extern char LIST_DIR[MAX_LEN_BUFFER];

void execute_dir_args(DIR *__d);

#endif //OSISP_LAB8_UTILS_H
