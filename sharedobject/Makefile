CC=gcc
CFLAGS=-ggdb -Wall
BINS=testlibmycode libmystaticcode.a libmycode.so runtime_librarytest static_librarytest

all: $(BINS)

libmycode.o: libmycode.c mycode.h
	$(CC) $(CFLAGS) -c libmycode.c

# fPIC = Position Independent Code
# shared = make shared object
libmycode.so: libmycode.c mycode.h
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libmycode.c -lc

testlibmycode: testlibmycode.c libmycode.o 
	$(CC) $(CFLAGS) -o $@ $^

# -lmycode ist kurzform fuer -libmycode (compiler kann das anscheinend selber auffüllen)
# LD_LIBRARY_PATH muss dann vor das executable gesetzt werden oder die library muss in einen der Libary-Pfade
# denn Standard findet man mittels
# ld --verbose | grep SEARCH_DIR | tr -s ' ;' \\012
#
# bei einem statischen link sieht man einen Einstiegspunkt für die Funktion reverse 
#$ objdump -t testlibmycode | grep reverse
#00000000000011e6 g     F .text	000000000000009b              reverse
# bei einem shared object ist die library noch nicht geladen, d.h. man sieht hier kein Einstiegspunkt
#$ object$ objdump -t runtime_librarytest | grep reverse
#0000000000000000       F *UND*	0000000000000000              reverse
#
runtime_librarytest: testlibmycode.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -lmycode

# archivfile
#   rcs = replace(r) create(c) index(s)
libmystaticcode.a: libmycode.o
	ar rcs libmystaticcode.a libmycode.o

static_librarytest: testlibmycode.c
	$(CC) $(CFLAGS) -o $@ $^ -L. -lmystaticcode


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm *.o $(BINS)
	@echo "Finished cleaning."
