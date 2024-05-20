#include "index.h"
#include <stdio.h>
#include <stdlib.h>

void handle_file(const char* filename);
index_hdr_s* read_index_header(FILE *file);
void read_index_records(FILE *file, index_hdr_s* header);
void print_time_marks(const index_hdr_s* header);
void cleanup(index_hdr_s* header, FILE *file);

int main(int argc, char* argv[]) {
    if (argc > 1) {
        handle_file(argv[1]);
    }
    return 0;
}

void handle_file(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    index_hdr_s* header = read_index_header(file);
    if (header == NULL) {
        fclose(file);
        exit(EXIT_FAILURE);
    }

    if (header->records > 0) {
        read_index_records(file, header);
    }

    print_time_marks(header);
    cleanup(header, file);
}

index_hdr_s* read_index_header(FILE *file) {
    index_hdr_s* header = (index_hdr_s*)malloc(sizeof(index_hdr_s));
    if (header == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }
    if (fread(&header->records, sizeof(uint64_t), 1, file) != 1) {
        fprintf(stderr, "Error reading records.\n");
        free(header);
        return NULL;
    }
    header->index = NULL;
    return header;
}

void read_index_records(FILE *file, index_hdr_s* header) {
    header->index = (index_record*)malloc(header->records * sizeof(index_record));
    if (header->index == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        free(header);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    if (fread(header->index, sizeof(index_record), header->records, file) != header->records) {
        fprintf(stderr, "Error reading indexes.\n");
        free(header->index);
        free(header);
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

void print_time_marks(const index_hdr_s* header) {
    for (uint16_t i = 0; i < header->records; ++i) {
        printf("%lf\n", header->index[i].time_mark);
    }
}

void cleanup(index_hdr_s* header, FILE *file) {
    if (header != NULL) {
        free(header->index);
        free(header);
    }
    if (file != NULL) {
        fclose(file);
    }
}
