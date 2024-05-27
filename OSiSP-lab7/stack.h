#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct stackNode {
    pthread_t threadId;
    struct stackNode* next;
} stackNode;

void push_stack(stackNode **head, pthread_t threadId);
void pop_stack(stackNode **head);
