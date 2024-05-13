CC=gcc
#CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BINDIR=bin
BIN=$(BINDIR)/tcp_client

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
