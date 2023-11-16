CC=gcc
CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv -lglib-2.0 
TARGETSPRG=gridtravel.c 
TARGETSTST=ktest.c

all: gridtravel

gridtravel: $(TARGETSPRG)
	$(CC) $(CFLAGS) $^ -o $@

ktest: $(TARGETSTST)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f gridtravel ktest
	@echo "Finished cleaning."
