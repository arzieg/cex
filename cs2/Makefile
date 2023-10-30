CC=gcc
CFLAGS=-ggdb -c -Wall 
SERVER_TARGETS=server.o
CLIENT_TARGETS=client.o

all: server client

server: $(SERVER_TARGETS)
	$(CC) $^ -o $@

client: $(CLIENT_TARGETS)
	$(CC) $^ -o $@


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f client server *.o
	@echo "Finished cleaning."
