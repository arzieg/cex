CC=gcc
CFLAGS=-ggdb -Wall -ftrapv
TARGETS=gridtravel.c kv.c

all: gridtravel

fib2: $(TARGETS)
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f gridtravel gridtravel.o kv.o
	@echo "Finished cleaning."
