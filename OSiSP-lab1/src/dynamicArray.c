#include "dynamicArray.h"

void initArray(struct DynamicArray* array, size_t initCapacity) {
    array->data = (char**)malloc(initCapacity * sizeof(char*));
    array->size = 0;
    array->capacity = initCapacity;
}

void freeArray(struct DynamicArray* array) {
    for (size_t i = 0; i < array->size; ++i) {
        free(array->data[i]);
    }
    free(array->data);
}

void resizeArray(struct DynamicArray* array, size_t newCapacity) {
    array->data = (char**)realloc(array->data, newCapacity * sizeof(char*));
    array->capacity = newCapacity;
}

void appendArray(struct DynamicArray* array, const char* value) {
    if (array->size == array->capacity) {
        size_t new_capacity = array->capacity * 2;
        resizeArray(array, new_capacity);
    }

    size_t length = strlen(value);
    char* new_string = (char*)malloc((length + 1) * sizeof(char));
    strcpy(new_string, value);
    array->data[array->size] = new_string;
    array->size++;
}

const char* getElementArray(const struct DynamicArray* array, size_t index) {
    if (index < array->size) {
        return array->data[index];
    } else {
        printf("Error: Index out of range\n");
        exit(1);
    }
}

void printArray(const struct DynamicArray* array) {
    for (size_t i = 0; i < array->size; ++i) {
        printf("%s %lu\n", array->data[i], strlen(getElementArray(array, i)));
    }
}

int comparator(const void* a, const void* b) {
    const char* str1 = *(const char**)a;
    const char* str2 = *(const char**)b;
    return strcmp(str1, str2);
}