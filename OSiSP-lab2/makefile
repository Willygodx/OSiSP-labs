CC = gcc

CFLAGS =  -W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic

.PHONY: all
all: start

start:	child.o parent.o
	$(CC) $^ -o parent	

parent.o: src/parent.c
	$(CC) $(CFLAGS) $<

child.o: src/child.c
	$(CC) $(CFLAGS) $< 
	
clean:
	rm -rf *.o dirwalks