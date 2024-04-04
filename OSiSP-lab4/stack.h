#include <stdio.h>
#include <stdlib.h>

typedef struct StackNode {
    pid_t pid;
    struct StackNode* next;
} StackNode;

void pushStack(StackNode** head, pid_t pid);
void popStack(StackNode** head);
