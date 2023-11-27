CC=gcc
CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv  
TARGET=example.c

all: example

example: $(TARGET)
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f example
	@echo "Finished cleaning."
