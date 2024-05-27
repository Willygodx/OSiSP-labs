#ifndef OSISP_LAB8_SERVER_H
#define OSISP_LAB8_SERVER_H

#include <stdbool.h>
#include <netdb.h>

#define MAX_LEN_BUFFER 1024
#define ECHO_QUERY "ECHO"
#define QUIT_QUERY "QUIT"
#define INFO_QUERY "INFO"
#define LIST_QUERY "LIST"
#define CD_QUERY "CD"
#define UNKNOWN_QUERY "UNKNOWN"
#define START_MESSAGE "myclient server.AF_INET\n"
#define EXIT_MESSAGE "The session has ended. Goodbye.\n"
#define WRONG_PARAMETER "You entered an incorrect parameter.\nList of available parameters: [CD, ECHO, LIST, QUIT, INFO].\n"


extern char QUERY[MAX_LEN_BUFFER];
extern bool is_new_dir;

#endif //OSISP_LAB8_SERVER_H
