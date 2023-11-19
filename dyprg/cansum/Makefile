CC=gcc
CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv  
TARGETSPRG=cansum.c
TARGETSTST=cansum2.c

all: cansum cansum2

cansum: $(TARGETSPRG)
	$(CC) $(CFLAGS) $^ -o $@

cansum2: $(TARGETSTST)
	$(CC) $(CFLAGS) $^ -lglib-2.0 -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f cansum cansum2
	@echo "Finished cleaning."
