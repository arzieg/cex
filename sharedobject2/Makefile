CC=gcc
CFLAGS=-ggdb -Wall
BINS=libplus1.so libplus2.so test

all: $(BINS)

%.so: %.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^

test: test.c
	$(CC) $(CFLAGS) -o $@ $^ -ldl

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm $(BINS)
	@echo "Finished cleaning."
