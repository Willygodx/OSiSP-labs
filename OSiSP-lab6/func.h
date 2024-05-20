#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pthread_barrier.h"
#include <pthread.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

typedef struct {
    index_record * buf;
    int block_size;
    int thread_number;
} thread_args;

typedef struct {
    int block_size;
    int threads;
    char* file_name;
} create_data;

extern int size;
extern int blocks;
extern int threads;

extern pthread_barrier_t barrier;
extern pthread_mutex_t mutex;
extern index_record* current;

int comparator(const void* a, const void* b);
void* block_sorting(void* thread_a);
void *open_memory(void* data);
