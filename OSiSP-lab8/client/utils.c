#include "utils.h"

char BUFFER[MAX_LEN_BUFFER];
char CORRECT_BUFFER[MAX_LEN_BUFFER];
char QUERY[MAX_LEN_BUFFER];

bool is_end_session() {
    if (strncasecmp(QUERY, QUIT_QUERY, strlen(QUIT_QUERY)) == 0) {
        return true;
    }
    return false;
}
