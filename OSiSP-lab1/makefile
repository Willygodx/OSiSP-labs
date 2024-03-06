CC = gcc

CFLAGS = -c -W -Wall -Wextra

.PHONY: all
all: start

start:	dirwalks.o dynamicArray.o directoryReading.o
	$(CC) $^ -o dirwalks	

dirwalks.o: src/dirwalks.c src/dynamicArray.h src/directoryReading.h
	$(CC) $(CFLAGS) $<

dynamicArray.o: src/dynamicArray.c src/dynamicArray.h
	$(CC) $(CFLAGS) $< 

directoryReading.o: src/directoryReading.c src/directoryReading.h
	$(CC) $(CFLAGS) $< 
	
clean:
	rm -rf *.o dirwalks