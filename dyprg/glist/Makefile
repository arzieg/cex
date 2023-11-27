CC=gcc
CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv  
SRC=src
BIN=bin

SRCS=$(wildcard $(SRC)/*.c)
BINS=$(SRCS:$(SRC)/%.c=%)

all: $(BINS)

%: $(SRC)/%.c
	$(CC) $(CFLAGS) -o $(BIN)/$@ $< -lglib-2.0 

.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) $(BIN)/*
	@echo "Finished cleaning."

