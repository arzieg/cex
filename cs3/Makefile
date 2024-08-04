CC=gcc
#CFLAGS=-lncurses -ggdb -Wall -ftrapv
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g 
LFLAGS=
SRC=src
OBJ=obj
#SRCS=$(wildcard $(SRC)/*.c)
#OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BINDIR=bin
#BIN=$(BINDIR)/tcp_server_toupper

all: srvprg

srvprg: SRCS=$(SRC)/srvprg.c
srvprg: OBJS=$(OBJ)/srvprg.o
srvprg: BIN=$(BINDIR)/srvprg
srvprg: clean
srvprg:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)


.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BINDIR)/* $(OBJ)/*
	@echo "Finished cleaning."
