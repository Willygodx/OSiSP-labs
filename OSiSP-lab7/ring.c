#include "ring.h"

void push(Node** head, Node** tail) {
    if (*head != NULL) {
        Node *temp = (Node *) malloc(sizeof(Node));
        temp->message = (Message*)malloc(sizeof(Message));
        init_message(temp->message);
        temp->next = *head;
        temp->prev = *tail;
        (*tail)->next = temp;
        (*head)->prev = temp;
        *tail = temp;
    } else {
        *head = (Node *) malloc(sizeof(Node));
        (*head)->message = (Message*)malloc(sizeof(Message));
        init_message((*head)->message);
        (*head)->prev = *head;
        (*head)->next = *head;
        *tail = *head;
    }
}
void pop(Node** head, Node** tail) {
    if (*head != NULL) {
        if (*head != *tail) {
            Node *temp = *head;
            (*tail)->next = (*head)->next;
            *head = (*head)->next;
            (*head)->prev = *tail;
            free(temp);
        } else {
            free(*head);
            *head = NULL;
            *tail = NULL;
        }
    }
}

void init_message(Message* message) {
    message->type = 0;
    message->hash = 0;
    message->size = rand() % 257;
    message->data = (u_int8_t*)malloc(message->size*sizeof(u_int8_t));
    for (size_t i = 0; i < message->size; i++) {
        message->data[i] = rand() % 256;
        message->hash += message->data[i];
    }
    message->hash = ~message->hash;
}

void print_message(Message* mes) {
    printf("Message type: %d, hash: %d, size: %d, data: ", mes->type, mes->hash, mes->size);
    for(size_t i = 0; i<mes->size; i++)
        printf("%d", mes->data[i]);
    printf("\n");
}
