CC=gcc
CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv -lglib-2.0 
DEPS=kv.h 
OBJ=kv.o 
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
