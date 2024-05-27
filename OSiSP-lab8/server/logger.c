#include "logger.h"

void logging_console(int fd_client, const char *__command, const char *__result, unsigned int __loger_level) {
    const time_t date_logging = time(NULL);
    struct tm *u = localtime(&date_logging);
    char str_date[LEN_DATA];
    strftime(str_date, 40, "%d.%m.%Y %H:%M:%S ", u);
    switch (__loger_level) {
        case __FATAL: {
            printf("\033[1;31m%sFATAL\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __ERROR: {
            printf("\033[1;31m%sERROR\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __WARN: {
            printf("\033[1;33m%sWARN\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __INFO: {
            printf("\033[1;32m%sINFO\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __DEBUG: {
            printf("\033[1;34m%sDEBUG\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        case __TRACE: {
            printf("\033[1;36m%sTRACE\033[0m %s from [%d] with arguments = %s",
                   str_date, __command, fd_client, __result);
            break;
        }
        default: break;
    }
}
