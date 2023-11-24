CC=gcc
CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv  
TARGETSPRG=howsum.c
TARGETSTST=howsum2.c
TARGETMP=glist.c

all: howsum 

howsum: $(TARGETSPRG)
	$(CC) $(CFLAGS) $^ -o $@

howsum2: $(TARGETSTST)
	$(CC) $(CFLAGS) $^ -lglib-2.0 -o $@

glist: $(TARGETMP)
	$(CC) $(CFLAGS) $^ -lglib-2.0 -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f howsum howsum2 glist
	@echo "Finished cleaning."
