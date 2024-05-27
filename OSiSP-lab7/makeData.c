#include "data.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 20

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("Please provide the file name as an argument.\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "ab+");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    record_s entry;
    while (1) {
        printf("Enter name: ");
        fgets(entry.name, sizeof(entry.name), stdin);
        entry.name[strcspn(entry.name, "\n")] = '\0'; // Remove newline character

        printf("Enter address: ");
        fgets(entry.address, sizeof(entry.address), stdin);
        entry.address[strcspn(entry.address, "\n")] = '\0'; // Remove newline character

        printf("Enter semester: ");
        scanf("%hhu", &entry.semester);

        fwrite(&entry, sizeof(record_s), 1, file);

        clear_input_buffer();
        printf("Enter 'w' to continue: ");
        if (getchar() != 'w') {
            break;
        }
        clear_input_buffer();
    }

    fclose(file);
    return 0;
}
