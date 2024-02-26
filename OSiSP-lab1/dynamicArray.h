#ifndef OSISP_LAB1_DYNAMICARRAY_H
#define OSISP_LAB1_DYNAMICARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct DynamicArray {
    char** data;
    size_t size;
    size_t capacity;
} DynamicArray;

void initArray (struct DynamicArray* array, size_t initCapacity);
void freeArray (struct DynamicArray* array);
void resizeArray (struct DynamicArray* array, size_t newCapacity);
void appendArray (struct DynamicArray* array, const char* value);
const char* getElementArray (const struct DynamicArray* array, size_t index);
void printArray (const struct DynamicArray* array);
int comparator (const void* a, const void* b);



#endif //OSISP_LAB1_DYNAMICARRAY_H
