#ifndef OSISP_LAB1_DIRECTORYREADING_H
#define OSISP_LAB1_DIRECTORYREADING_H

#include <dirent.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include "dynamicArray.h"

typedef struct flagOptions
{
    int SOFT_LINKS_FLAG;
    int DIRECTORIES_FLAG;
    int FILES_FLAG;
    int SORT_FLAG;

} FlagOptions;

void recursiveRead(const char* path, FlagOptions* flags, DynamicArray* array);

#endif //OSISP_LAB1_DIRECTORYREADING_H
