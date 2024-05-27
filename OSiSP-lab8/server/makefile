CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic

SERVER_SRC=server.c utils.c file.c logger.c network.c

SERVER_OBJ=$(SERVER_SRC:.c=.o)

SERVER_TARGET=server

all: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $(SERVER_OBJ) -o $(SERVER_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(SERVER_OBJ) $(SERVER_TARGET)
