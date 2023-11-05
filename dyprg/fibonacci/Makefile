CC=gcc
CFLAGS=-ggdb -c -Wall 
TARGETS=kv.o kvs.o

all: kv

kv: $(TARGETS)
	$(CC) $^ -o $@


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f client kv kv.o
	@echo "Finished cleaning."
