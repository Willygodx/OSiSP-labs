#include "file.h"
#include <stdio.h>
#include <stdlib.h>

void execute_file_args(const char *__filename, char **__buffer, size_t *__size) {
    FILE *file = fopen(__filename, "r");
    if (file == NULL) {
        printf("The file was not found. Check the data is correct.\n");
        return;
    }
    if (__buffer == NULL) {
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_END);
    *__size = ftell(file);
    fseek(file, 0, SEEK_SET);
    *__buffer = (char *) realloc(*__buffer, *__size * sizeof(char));
    fread(*__buffer, sizeof(char), *__size, file);
    fclose(file);
}
