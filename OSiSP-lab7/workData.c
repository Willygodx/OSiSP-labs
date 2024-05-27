#include "dataOperations.h"

int fileDescriptor;

int main(int argc, char* argv[]) {
    if(argc > 1) {
        if((fileDescriptor = open(argv[1], O_RDWR | O_CREAT, 0666)) == -1) {
            perror("Error opening file");
            return 1;
        }

        record_s* currentRecord = NULL;
        int running = 1;

        while(running) {
            printf("1. List data\n2. Get element\n3. Change element\n4. Save edited element\n5. Exit: ");
            char option = getchar();
            while(getchar() != '\n')
                continue;
            switch(option) {
                case '1':
                    displayRecords();
                    break;
                case '2':
                    currentRecord = fetchRecordByIndex();
                    break;
                case '3':
                    modifyRecord(currentRecord);
                    break;
                case '4':
                    storeRecord(currentRecord);
                    break;
                case '5':
                    running = 0;
                    break;
                default:
                    printf("Invalid option\n");
                    break;
            }
        }

        close(fileDescriptor);
    } else {
        printf("Enter file name.\n");
    }
    return 0;
}
