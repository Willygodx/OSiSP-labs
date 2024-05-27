#include "dataOperations.h"

int indexRecord = -1;
record_s* recordBackup = NULL;

void displayRecords() {
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;

    if(fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        perror("Error acquiring read lock");
        close(fileDescriptor);
        exit(errno);
    }

    lseek(fileDescriptor, 0, SEEK_SET);

    int recordIndex = 1;
    record_s currentRecord;
    while(read(fileDescriptor, &currentRecord, sizeof(record_s)) > 0) {
        printf("Record %d: name - %s, address - %s, semester - %hhu.\n",
               recordIndex++, currentRecord.name, currentRecord.address, currentRecord.semester);
    }

    lock.l_type = F_UNLCK;
    if(fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        perror("Error releasing read lock");
        close(fileDescriptor);
        exit(errno);
    }
}

record_s *fetchRecordByIndex() {
    printf("Enter index: ");
    scanf("%d", &indexRecord);
    while(getchar() != '\n') continue;

    record_s *record = (record_s *) malloc(sizeof(record_s));

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_len = sizeof(record_s);
    lock.l_start = (indexRecord - 1) * sizeof(record_s);
    lock.l_whence = SEEK_SET;

    if(fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        perror("Error acquiring read lock");
        close(fileDescriptor);
        exit(errno);
    }

    lseek(fileDescriptor, (indexRecord - 1) * sizeof(record_s), SEEK_SET);

    if(read(fileDescriptor, record, sizeof(record_s)) == 0) {
        perror("Error reading record");
        close(fileDescriptor);
        exit(errno);
    }

    lock.l_type = F_UNLCK;
    if(fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        perror("Error releasing read lock");
        close(fileDescriptor);
        exit(errno);
    }

    if(recordBackup != NULL)
        free(recordBackup);
    recordBackup = (record_s *)malloc(sizeof(record_s));
    strcpy(recordBackup->name, record->name);
    strcpy(recordBackup->address, record->address);
    recordBackup->semester = record->semester;

    printf("Record: name - %s, address - %s, semester - %hhu.\n",
           record->name, record->address, record->semester);
    printf("Edit?\n1) Yes\n2) No:");
    if(getchar() == '1') {
        while(getchar() != '\n') continue;
        modifyRecord(record);
    }
    while(getchar() != '\n') continue;

    return record;
}

void modifyRecord(record_s* record) {
    if(record == NULL) {
        printf("No record to modify.\n");
        return;
    }

    printf("Modify record: name - %s, address - %s, semester - %hhu.\n",
           record->name, record->address, record->semester);

    int editing = 1;
    while(editing) {
        printf("Modify:\n1. Name\n2. Address\n3. Semester\n4. Exit:");
        char choice = getchar();
        while(getchar() != '\n') continue;
        switch(choice) {
            case '1':
                printf("Input name: ");
                fgets(record->name, 80, stdin);
                record->name[strlen(record->name) - 1] = '\0';
                break;
            case '2':
                printf("Input address: ");
                fgets(record->address, 80, stdin);
                record->address[strlen(record->address) - 1] = '\0';
                break;
            case '3':
                printf("Input semester: ");
                scanf("%hhu", &record->semester);
                while(getchar() != '\n') continue;
                break;
            case '4':
                editing = 0;
                break;
        }
    }

    printf("Save changes?\n1. Yes\n2. No:");
    if(getchar() == '1') {
        while(getchar() != '\n') continue;
        storeRecord(record);
    }
    while(getchar() != '\n') continue;
}

void storeRecord(record_s* record) {
    if(record == NULL) {
        printf("No record to store.\n");
        return;
    }

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_len = sizeof(record_s);
    lock.l_start = (indexRecord - 1) * sizeof(record_s);
    lock.l_whence = SEEK_SET;

    if(fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        perror("Error acquiring read lock");
        close(fileDescriptor);
        exit(errno);
    }

    lseek(fileDescriptor, (indexRecord - 1) * sizeof(record_s), SEEK_SET);

    record_s *tempRecord = (record_s *) malloc(sizeof(record_s));
    if(read(fileDescriptor, tempRecord, sizeof(record_s)) == 0) {
        perror("Error rereading record");
        close(fileDescriptor);
        exit(errno);
    }

    lock.l_type = F_UNLCK;
    if(fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        perror("Error releasing read lock");
        close(fileDescriptor);
        exit(errno);
    }

    if(strcmp(recordBackup->name, tempRecord->name) != 0 ||
       strcmp(recordBackup->address, tempRecord->address) != 0 ||
       recordBackup->semester != tempRecord->semester) {

        printf("Record has been modified externally, current data:\n");
        printf("Record: name - %s, address - %s, semester - %hhu.\n",
               tempRecord->name, tempRecord->address, tempRecord->semester);
        *record = *tempRecord;

        strcpy(recordBackup->name, tempRecord->name);
        strcpy(recordBackup->address, tempRecord->address);
        recordBackup->semester = tempRecord->semester;

        modifyRecord(record);
        return;
    }

    lock.l_type = F_WRLCK;
    if(fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        perror("Error acquiring write lock");
        close(fileDescriptor);
        exit(errno);
    }

    lseek(fileDescriptor, (indexRecord - 1) * sizeof(record_s), SEEK_SET);

    if(write(fileDescriptor, record, sizeof(record_s)) == 0) {
        perror("Error writing record");
        close(fileDescriptor);
        exit(errno);
    }

    lock.l_type = F_UNLCK;
    if(fcntl(fileDescriptor, F_SETLKW, &lock) == -1) {
        perror("Error releasing write lock");
        close(fileDescriptor);
        exit(errno);
    }

    printf("Changes saved successfully.\n");
}
