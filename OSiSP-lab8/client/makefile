CC=gcc
CFLAGS=-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic

CLIENT_SRC=client.c network.c processing.c utils.c

CLIENT_OBJ=$(CLIENT_SRC:.c=.o)

CLIENT_TARGET=client

all: $(CLIENT_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) -o $(CLIENT_TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(CLIENT_OBJ) $(CLIENT_TARGET)
