#include "directoryReading.h"

int main(int argc, char *argv[])
{
    int option;
    DynamicArray array;
    initArray(&array, 10);
    FlagOptions flags= {0, 0, 0, 0};

    char* directory = "./";

    while((option = getopt(argc, argv, "ldfs")) != -1) {
        switch (option) {
            case 'l':
                printf("Option -l is set\n");
                flags.SOFT_LINKS_FLAG = 1;
                break;

            case 'd':
                printf("Option -d is set\n");
                flags.DIRECTORIES_FLAG = 1;
                break;

            case 'f':
                flags.FILES_FLAG = 1;
                printf("Option -f is set\n");
                break;

            case 's':
                flags.SORT_FLAG = 1;
                printf("Option -s is set\n");
                break;

            default:
                break;

        }
    }

    if(optind < argc)
        directory = argv[optind];
    printf("Directory: %s\n", directory);

    recursiveRead(directory, &flags, &array);
    if(flags.SORT_FLAG)
        qsort(array.data, array.size, sizeof(char *), comparator);

    printArray(&array);

    return 0;
}