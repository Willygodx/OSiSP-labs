#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LENGTH 256
#define CHILD_NAME_LENGTH 9
#define ENV_COUNT 11
#define _GNU_SOURCE

extern char **environ;

void sortStrings(char ***strings, size_t rows);
void printStrings(char **strings, size_t rows);
char *findEnv(char **env, char *key, size_t rows);

int main(int argc, char *argv[], char *envp[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Error! File is not provided\n");
        exit(EXIT_FAILURE);
    }

    size_t sizeOfEnv = 0;
    while (environ[sizeOfEnv])
        sizeOfEnv++;

    sortStrings(&environ, sizeOfEnv);
    printStrings(environ, sizeOfEnv);

    char *childEnvPath = argv[1];
    size_t childIndex = 0;

    while (1)
    {
        setenv("LC_COLLATE", "C", 1);
        printf("Input char: + * & q.\n>");
        rewind(stdin);
        char opt;
        while (1)
        {
            scanf("%c", &opt);
            if (opt == '+' || opt == '*' || opt == '&' || opt == 'q')
            {
                break;
            }
        }

        char childName[CHILD_NAME_LENGTH];
        snprintf(childName, CHILD_NAME_LENGTH, "%s%02d", "child_", (int)childIndex);
        char *childArgs[] = {childName, childEnvPath, NULL};
        int statusChild = 0;

        pid_t pid;
        switch (opt)
        {
            case '+':
                pid = fork();
                if (pid == -1)
                {
                    printf("Error! Error occurred, error code - %d\n", errno);
                    exit(errno);
                }
                if (pid == 0)
                {
                    char *CHILD_PATH = getenv("CHILD_PATH");
                    printf("%s\n", CHILD_PATH);
                    printf("Child process created. Please, wait...\n");
                    execve(CHILD_PATH, childArgs, environ);
                }
                wait(&statusChild);
                break;

            case '*':
                pid = fork();
                if (pid == -1)
                {
                    printf("Error! Error occurred, error code - %d\n", errno);
                    exit(errno);
                }
                if (pid == 0)
                {
                    char *CHILD_PATH = findEnv(envp, "CHILD_PATH", sizeOfEnv);
                    printf("Child process created. Please, wait...\n");
                    execve(CHILD_PATH, childArgs, environ);
                }
                wait(&statusChild);
                break;

            case '&':
                pid = fork();
                if (pid == -1)
                {
                    printf("Error! Error occurred, error code - %d\n", errno);
                    exit(errno);
                }
                if (pid == 0)
                {
                    char *CHILD_PATH = findEnv(environ, "CHILD_PATH", sizeOfEnv);
                    printf("Child process created. Please, wait...\n");
                    execve(CHILD_PATH, childArgs, environ);
                }
                wait(&statusChild);
                break;

            case 'q':
                exit(EXIT_SUCCESS);

            default:
                break;
        }

        childIndex++;
        if (childIndex > 99)
            childIndex = 0;
    }
}

void printStrings(char **strings, size_t rows)
{
    for (size_t i = 0; i < rows; i++)
        fprintf(stdout, "%s\n", strings[i]);
}

void swap(char **s1, char **s2)
{
    char *tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

void sortStrings(char ***strings, size_t rows)
{
    for (size_t i = 0; i < rows - 1; i++)
    {
        for (size_t j = 0; j < rows - i - 1; j++)
        {
            if (strcoll((*strings)[j], (*strings)[j + 1]) > 0)
                swap(&((*strings)[j]), &((*strings)[j + 1]));
        }
    }
}

char *findEnv(char **env, char *key, size_t rows)
{
    char *result = calloc(MAX_LENGTH, sizeof(char));
    for (size_t i = 0; i < rows; i++)
    {
        if (strstr(env[i], key))
            strncpy(result, env[i] + strlen(key) + 1, MAX_LENGTH);
    }
    return result;
}
