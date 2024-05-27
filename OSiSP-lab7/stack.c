#include "stack.h"

void push_stack(stackNode** head, pthread_t threadId) {
    stackNode *new = (stackNode *) malloc(sizeof(stackNode));
    new->next = *head;
    new->threadId = threadId;
    *head = new;
}
void pop_stack(stackNode** head) {
    if(*head!=NULL) {
        stackNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}
