CC=gcc
CFLAGS=-ggdb -Wall 
TARGETS=fib2.c kv.c

all: fib2

fib2: $(TARGETS)
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f client fib2 fib2.o kv.o
	@echo "Finished cleaning."
