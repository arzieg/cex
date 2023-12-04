CC=gcc
CFLAGS=-ggdb -Wall
BINS=randtest shim.so

all: $(BINS)

%.so: %.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^ -ldl

randtest: randtest.c
	$(CC) $(CFLAGS) -o $@ $^ 

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm $(BINS)
	@echo "Finished cleaning."
