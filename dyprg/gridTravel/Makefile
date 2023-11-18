CC=gcc
CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv  
TARGETSPRG=gridtravel2.c 
TARGETSTST=ktest.c

all: gridtravel2

gridtravel2: $(TARGETSPRG)
	$(CC) $(CFLAGS) $^ -lglib-2.0 -o $@

ktest: $(TARGETSTST)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f gridtravel ktest gridtravel2
	@echo "Finished cleaning."
