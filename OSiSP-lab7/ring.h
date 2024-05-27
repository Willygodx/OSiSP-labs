#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

extern int MAX_MES_COUNT;

typedef struct {
    u_int8_t type;
    u_int16_t hash;
    u_int8_t size;
    u_int8_t* data;
} Message;

typedef struct node {
    Message* message;
    struct node* next;
    struct node* prev;
} Node;

typedef struct {
    Node* ringHead;
    Node* ringTail;
    int countAdded;
    int countDeleted;
} Queue;

void push(Node**, Node**);
void pop(Node**, Node**);

void init_message(Message *message);
void print_message(Message *mes);
