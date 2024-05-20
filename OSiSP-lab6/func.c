#include "func.h"

void *open_memory(void* data) {
    printf("Opening data start.\n");
    create_data* cData = (create_data*)data;

    FILE* f = fopen(cData->file_name, "rb+");
    if(f==NULL){
        printf("Error while open file.\n");
        return 0;
    }

    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    int fd = fileno(f);
    void * buf;
    if((buf = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        printf("Error while creating mapping, %d\n", errno);
        exit(errno);
    }

    buf = (char*)buf + sizeof(uint64_t);

    current = (index_record *)buf;
    printf("Creating threads.\n");
    pthread_t threadsId[cData->threads-1];
    for(int i = 1; i<cData->threads; i++) {
        thread_args* args = (thread_args*)malloc(sizeof(thread_args));
        args->block_size = cData->block_size;
        args->thread_number = i;
        args->buf = (index_record*)buf;

        if(pthread_create(&threadsId[i - 1], NULL, block_sorting, args) != 0) {
            printf("Error while creating %d thread.\n", i);
            exit(0);
        }
    }
    thread_args* args = (thread_args*)malloc(sizeof(thread_args));
    args->block_size = cData->block_size;
    args->thread_number = 0;
    args->buf = (index_record*)buf;
    block_sorting(args);

    for(int i = 1; i<cData->threads; i++)
        pthread_join(threadsId[i-1], NULL);
    printf("End of threads.\n");

    munmap(buf, file_size);
    fclose(f);
    printf("Memory closing.\n");
    exit(0);
}

void* block_sorting(void* thread_a) {
    thread_args* args = (thread_args*)thread_a;
    pthread_barrier_wait(&barrier);

    printf("Qsort in %d thread.\n", args->thread_number);
    while(current < args->buf + size) {
        pthread_mutex_lock(&mutex);
        if(current < args->buf + size) {
            index_record *temp = current;
            current += args->block_size;
            pthread_mutex_unlock(&mutex);
            qsort(temp, args->block_size, sizeof(index_record), comparator);
        } else {
            pthread_mutex_unlock(&mutex);
            pthread_barrier_wait(&barrier);
            break;
        }
    }
    printf("Merging in %d thread.\n", args->thread_number);
    int mergeStep = 2;

    while(mergeStep<=blocks) {
        pthread_barrier_wait(&barrier);
        current = args->buf;
        while (current < args->buf + size) {
            pthread_mutex_lock(&mutex);
            if(current < args->buf + size) {
                index_record *temp = current;
                current += mergeStep * args->block_size;
                pthread_mutex_unlock(&mutex);
                int bufSize = (mergeStep / 2) * args->block_size;
                index_record *left = (index_record *) malloc(bufSize * sizeof(index_record));
                memcpy(left, temp, (mergeStep / 2) * args->block_size * sizeof(index_record));
                index_record *right = (index_record *) malloc(bufSize * sizeof(index_record));
                memcpy(right, temp + (mergeStep / 2) * args->block_size, (mergeStep / 2) * args->block_size * sizeof(index_record));

                int i = 0, j = 0;
                while (i < bufSize && j < bufSize) {
                    if (left[i].time_mark < right[j].time_mark) {
                        temp[i+j].time_mark = left[i].time_mark;
                        temp[i+j].recno = left[i].recno;
                        i++;
                    } else {
                        temp[i+j].time_mark = right[j].time_mark;
                        temp[i+j].recno = right[j].recno;
                        j++;
                    }
                }
                if (i == bufSize) {
                    while (j > bufSize) {
                        temp[i+j].time_mark = right[j].time_mark;
                        temp[i+j].recno = right[j].recno;
                        j++;
                    }
                }
                if (j == bufSize) {
                    while (i < bufSize) {
                        temp[i+j].time_mark = left[i].time_mark;
                        temp[i+j].recno = left[i].recno;
                        i++;
                    }
                }
            } else {
                pthread_mutex_unlock(&mutex);
                break;
            }
        }
        mergeStep*=2;
    }
    pthread_mutex_unlock(&mutex);
    pthread_barrier_wait(&barrier);
    return NULL;
}

int comparator(const void* a, const void* b) {
    if (((index_record*)a)->time_mark < ((index_record*)b)->time_mark)
        return -1;
    else if(((index_record*)a)->time_mark > ((index_record*)b)->time_mark)
        return 1;
    else
        return 0;
}
