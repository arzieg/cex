CC=gcc
#CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv
CFLAGS=-ggdb -Wall -ftrapv
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BINDIR=bin
BIN=$(BINDIR)/main

LIBDIR=lib
LIB=$(LIBDIR)/hello.a

all: $(BIN) 

release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release: $(BIN) 

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BINDIR)/* $(OBJ)/*
	@echo "Finished cleaning."
