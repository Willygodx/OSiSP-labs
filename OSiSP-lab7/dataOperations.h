#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

extern int indexRecord;
extern record_s *recordBackup;
extern int fileDescriptor;

void displayRecords();
record_s *fetchRecordByIndex();
void modifyRecord(record_s*);
void storeRecord(record_s*);
