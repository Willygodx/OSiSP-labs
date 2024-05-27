#include "mainFunctions.h"

void *fill_messages(void *args) {
    pthread_t threadId = *((pthread_t *) args);
    while (continuingFiller) {
        pthread_mutex_lock(&mutex);

        while (queue->countAdded - queue->countDeleted >= MAX_MES_COUNT || !continuingFiller) {
            if (pthread_equal(stackFiller->threadId, threadId) && !continuingFiller)
                break;
            pthread_cond_wait(&condFill, &mutex);
        }

        if (queue->countAdded - queue->countDeleted < MAX_MES_COUNT) {
            push(&queue->ringHead, &queue->ringTail);
            queue->countAdded++;
            printf("Added %d message:\n", queue->countAdded);
            print_message(queue->ringTail->message);
        } else
            printf("Queue is full!\n");

        if (queue->countAdded - queue->countDeleted > 0)
            pthread_cond_broadcast(&condExtract);

        pthread_mutex_unlock(&mutex);
        if (!(pthread_equal(stackFiller->threadId, threadId) && !continuingFiller))
            sleep(1);
    }
    continuingFiller = 1;
    pthread_join(threadId, NULL);
    return NULL;
}

void *extract_messages(void* args) {
    pthread_t threadId = (pthread_t) args;
    while (continuingExtractor) {
        pthread_mutex_lock(&mutex);

        while (queue->countAdded - queue->countDeleted == 0 || !continuingExtractor) {
            if (pthread_equal(stackExtractor->threadId, threadId) && !continuingExtractor)
                break;
            pthread_cond_wait(&condExtract, &mutex);
        }

        if (queue->countAdded - queue->countDeleted > 0) {
            Message *temp = queue->ringHead->message;
            pop(&queue->ringHead, &queue->ringTail);
            queue->countDeleted++;
            printf("Delete %d message:\n", queue->countDeleted);
            print_message(temp);
            free(temp->data);
            free(temp);
        } else
            printf("Queue is empty!\n");

        if (queue->countAdded - queue->countDeleted < MAX_MES_COUNT)
            pthread_cond_broadcast(&condFill);

        pthread_mutex_unlock(&mutex);
        if (!(pthread_equal(stackExtractor->threadId, threadId) && !continuingExtractor))
            sleep(1);
    }
    continuingExtractor = 1;
    pthread_join(threadId, NULL);
    return NULL;
}

void stop_filling() {
    continuingFiller = 0;
}
void stop_extracting() {
    continuingExtractor = 0;
}
