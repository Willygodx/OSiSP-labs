#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "index.h"

#define MAX_SIZE 65536

void generate_index_records(index_record *records, int num_records) {
    srand(time(NULL));
    for (int i = 0; i < num_records; i++) {
        records[i].recno = i + 1;
        records[i].time_mark = 15020 + rand() % (60378 - 15020 + 1);
        records[i].time_mark += 0.5 * ((rand() % 24) * 60 * 60 + (rand() % 60) * 60 + rand() % 60) / (12 * 60 * 60);
    }
}

void write_index_file(const char *filename, index_hdr_s header) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error while creating file.\n");
        exit(EXIT_FAILURE);
    }
    fwrite(&header.records, sizeof(header.records), 1, file);
    fwrite(header.index, sizeof(index_record), header.records, file);
    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        index_hdr_s header;
        header.records = atoi(argv[1]);
        if (header.records % 256 != 0) {
            printf("Wrong size, it should be a multiple of 256.\n");
            exit(EXIT_FAILURE);
        }
        if (header.records > MAX_SIZE) {
            printf("Size exceeds maximum limit.\n");
            exit(EXIT_FAILURE);
        }
        header.index = (index_record *)malloc(header.records * sizeof(index_record));
        if (header.index == NULL) {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        generate_index_records(header.index, header.records);
        write_index_file(argv[2], header);
        free(header.index);
    } else {
        printf("Provide parameters.\n");
    }
    return 0;
}
