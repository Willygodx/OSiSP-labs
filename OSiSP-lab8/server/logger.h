#ifndef OSISP_LAB8_LOGGER_H
#define OSISP_LAB8_LOGGER_H

#include <stdio.h>
#include <time.h>

#define LEN_DATA 40

enum LEVEL_LOGGER {
    __FATAL = 0,
    __ERROR = 1,
    __WARN = 2,
    __INFO = 3,
    __DEBUG = 4,
    __TRACE = 5
};

void logging_console(int fd_client, const char *__command, const char *__result, unsigned int __loger_level);

#endif //OSISP_LAB8_LOGGER_H
