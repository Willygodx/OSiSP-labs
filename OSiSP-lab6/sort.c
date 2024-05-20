#include "func.h"

int size;
int blocks;
int threads;

pthread_barrier_t barrier;
pthread_mutex_t mutex;
index_record *current;

void initialize(int argc, char *argv[]) {
    if (argc > 1) {
        printf("Scanning params.\n");
        size = atoi(argv[1]);
        blocks = atoi(argv[2]);
        threads = atoi(argv[3]);
        if (size % 4096 != 0) {
            printf("Wrong size, it should be multiple 4096.\n");
            exit(0);
        }
        if (threads < 8 || threads > 8000) {
            printf("Wrong count of threads, from 8 to 8000.\n");
            exit(0);
        }
        if (blocks % 2 != 0 || blocks < threads) {
            printf("Wrong count of block, it should be multiple 2 and more than threads count.\n");
            exit(0);
        }

        if (pthread_barrier_init(&barrier, NULL, threads)) {
            printf("Error while creating barrier.\n");
            exit(0);
        }

        if (pthread_mutex_init(&mutex, NULL) != 0) {
            printf("Error while creating mutex.\n");
            exit(0);
        }
    } else {
        printf("Not enough parameters.\n");
        exit(0);
    }
}

void cleanup(void) {
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
    printf("Work done.\n");
}

int main(int argc, char *argv[]) {
    initialize(argc, argv);

    create_data *cd = (create_data *)malloc(sizeof(create_data));
    cd->block_size = size / blocks;
    cd->threads = threads;
    cd->file_name = argv[4];

    pthread_t pthreadId;
    if (pthread_create(&pthreadId, NULL, open_memory, cd) != 0) {
        printf("Error while creating 0 thread.\n");
        exit(0);
    }

    if (pthread_join(pthreadId, NULL) != 0) {
        printf("Error executing process.\n");
        exit(0);
    }

    cleanup();
    return 0;
}
