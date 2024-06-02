CC=gcc
#CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g
LFLAGS=-L/usr/lib/x86_64-linux-gnu -lcrypto -lssl
SRC=src
OBJ=obj
#SRCS=$(wildcard $(SRC)/*.c)
#OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BINDIR=bin
#BIN=$(BINDIR)/tls_time_server


release: SRCS=$(SRC)/tls_time_server.c
release: OBJS=$(OBJ)/tls_time_server.o
release: BIN=$(BINDIR)/tls_time_server
release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

tls_time_server: SRCS=$(SRC)/tls_time_server.c
tls_time_server: OBJS=$(OBJ)/tls_time_server.o
tls_time_server: BIN=$(BINDIR)/tls_time_server
tls_time_server:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)



.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BINDIR)/tls_time_server $(OBJ)/*
	@echo "Finished cleaning."
