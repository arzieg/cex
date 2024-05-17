CC=gcc
#CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g
SRC=src
OBJ=obj
#SRCS=$(wildcard $(SRC)/*.c)
#OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BINDIR=bin
#BIN=$(BINDIR)/tcp_server_toupper


release: SRCS=$(SRC)/tcp_server_toupper.c
release: OBJS=$(OBJ)/tcp_server_toupper.o
release: BIN=$(BINDIR)/tcp_server_toupper
release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

client: SRCS=$(SRC)/udp_client.c
client: OBJS=$(OBJ)/udp_client.o
client: BIN=$(BINDIR)/udp_client
client:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

dns_query: SRCS=$(SRC)/dns_query.c
dns_query: OBJS=$(OBJ)/dns_query.o
dns_query: BIN=$(BINDIR)/dns_query
dns_query:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

lookup: SRCS=$(SRC)/lookup.c
lookup: OBJS=$(OBJ)/lookup.o
lookup: BIN=$(BINDIR)/lookup
lookup:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BINDIR)/* $(OBJ)/*
	@echo "Finished cleaning."
