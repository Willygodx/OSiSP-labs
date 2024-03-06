#include <sys/param.h>
#include "directoryReading.h"

void recursiveRead(const char* path, FlagOptions* flags, DynamicArray* array)
{
    struct dirent *entry;
    struct stat fileInfo;
    DIR *dir;

    dir = opendir(path);
    if (!dir) {
        perror("opendir(""/"")");
        exit(errno);
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullPath[PATH_MAX];
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if (lstat(fullPath, &fileInfo) == -1) {
            perror("lstat");
            continue;
        }

        if ((S_IFDIR & fileInfo.st_mode) && flags->DIRECTORIES_FLAG) {
            appendArray(array, fullPath);
        } else if (S_ISLNK(fileInfo.st_mode) && flags->SOFT_LINKS_FLAG)
        {
            appendArray(array, fullPath);
        } else if((S_IFREG & fileInfo.st_mode) && flags->FILES_FLAG)
        {
            appendArray(array, fullPath);
        } else if (!flags->DIRECTORIES_FLAG && !flags->SOFT_LINKS_FLAG
                   && !flags->FILES_FLAG && ((S_IFREG & fileInfo.st_mode) || S_ISLNK(fileInfo.st_mode) || (S_IFDIR & fileInfo.st_mode)))
        {
            appendArray(array, fullPath);
        } else
            continue;

        if(entry->d_type == DT_DIR) {
            recursiveRead(fullPath, flags, array);
        }
    }
    closedir(dir);

}

