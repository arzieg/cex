CC=gcc
#CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g
LFLAGS=-L/usr/lib/x86_64-linux-gnu -lssh
SRC=src
OBJ=obj
#SRCS=$(wildcard $(SRC)/*.c)
#OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BINDIR=bin
#BIN=$(BINDIR)/ssh_connect


release: SRCS=$(SRC)/ssh_connect.c
release: OBJS=$(OBJ)/ssh_connect.o
release: BIN=$(BINDIR)/ssh_connect
release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

ssh_connect: SRCS=$(SRC)/ssh_connect.c
ssh_connect: OBJS=$(OBJ)/ssh_connect.o
ssh_connect: BIN=$(BINDIR)/ssh_connect
ssh_connect:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

ssh_command: SRCS=$(SRC)/ssh_command.c
ssh_command: OBJS=$(OBJ)/ssh_command.o
ssh_command: BIN=$(BINDIR)/ssh_command
ssh_command:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

ssh_download: SRCS=$(SRC)/ssh_download.c
ssh_download: OBJS=$(OBJ)/ssh_download.o
ssh_download: BIN=$(BINDIR)/ssh_download
ssh_download:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)


.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BINDIR)/ssh_connect $(OBJ)/*
	@echo "Finished cleaning."
