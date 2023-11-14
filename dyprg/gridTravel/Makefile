CC=gcc
CFLAGS=-ggdb -Wall -ftrapv
DEPS=kv.h kvs.h
OBJ=kv.o kvs.o
#TARGETS=gridtravel.c kv.c kvs.c

all: gridtravel

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gridtravel: $(OBJ)
	$(CC) $(CFLAGS) $^ gridtravel.c -o $@

ktest: $(OBJ)
	$(CC) $(CFLAGS) $^ ktest.c -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f gridtravel gridtravel.o kv.o kvs.o ktest ktest.o
	@echo "Finished cleaning."
