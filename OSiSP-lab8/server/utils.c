#include "utils.h"
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char CURRENT_DIR[MAX_LEN_BUFFER];
char LIST_DIR[MAX_LEN_BUFFER];

void execute_dir_args(DIR *__d) {
    struct dirent *dir = NULL;
    struct stat sb;
    memset(LIST_DIR, '\0', sizeof(LIST_DIR));
    strncat(LIST_DIR, CURRENT_DIR, strlen(CURRENT_DIR));
    strncat(LIST_DIR, "\n", strlen("\n"));
    while ((dir = readdir(__d))) {
        char fullpath[MAX_LEN_BUFFER];
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }
        sprintf(fullpath, "%s/%s", CURRENT_DIR, dir->d_name);
        if (lstat(fullpath, &sb) == -1) {
            continue;
        }
        if ((sb.st_mode & S_IFMT) == S_IFDIR) {
            strncat(LIST_DIR, dir->d_name, strlen(dir->d_name));
            strncat(LIST_DIR, "/", strlen("/"));
        } else if ((sb.st_mode & S_IFMT) == S_IFREG) {
            strncat(LIST_DIR, dir->d_name, strlen(dir->d_name));
        } else if ((sb.st_mode & S_IFMT) == S_IFLNK) {
            char target_path[MAX_LEN_BUFFER];
            ssize_t bytes = readlink(fullpath, target_path, sizeof(target_path) - 1);
            if (bytes != -1) {
                lstat(target_path, &sb);
                if ((sb.st_mode & S_IFMT) == S_IFDIR) {
                    strncat(LIST_DIR, dir->d_name, strlen(dir->d_name));
                    strncat(LIST_DIR, LINK_TO_FILE, strlen(LINK_TO_FILE));
                    strncat(LIST_DIR, target_path, strlen(target_path));
                } else if ((sb.st_mode & S_IFMT) == S_IFREG) {
                    strncat(LIST_DIR, dir->d_name, strlen(dir->d_name));
                    strncat(LIST_DIR, LINK_TO_LINK, strlen(LINK_TO_LINK));
                    strncat(LIST_DIR, target_path, strlen(target_path));
                }
            }
        }
        strncat(LIST_DIR, "\n", strlen("\n"));
    }
    closedir(__d);
}
