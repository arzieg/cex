CC=gcc
CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -ggdb -Wall -ftrapv  
TARGETSPRG=cansum.c
TARGETSTST=ktest.c

all: cansum

cansum: $(TARGETSPRG)
	$(CC) $(CFLAGS) $^ -o $@

ktest: $(TARGETSTST)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f cansum ktest
	@echo "Finished cleaning."
