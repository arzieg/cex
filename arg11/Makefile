CC=gcc
#CFLAGS=-Wpedantic -Wall -Wextra -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g
CFLAGS=-Wall -g
LFLAGS=
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
AOBJ=$(OBJ)/libdotdash.a
BINDIR=bin
BIN=$(BINDIR)/arg

all: 
	$(CC) $(CFLAGS) -c -o $(OBJ)/dotdash.o  $(SRC)/dotdash.c 
	$(CC) $(CFLAGS) -c $(SRC)/arg.c -o $(OBJ)/arg.o
	ar cru $(OBJ)/libdotdash.a $(OBJ)/dotdash.o
	ranlib $(OBJ)/libdotdash.a
	$(CC) $(CFLAGS) $(OBJ)/arg.o -L$(OBJ) -ldotdash -o $(BIN)



.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BINDIR)/* $(OBJ)/*
	@echo "Finished cleaning."
