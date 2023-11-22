CC=gcc
CFLAGS=-I/usr/include -ggdb -Wall -ftrapv  
EXE=readonly

all: $(EXE)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f *.dSYM *.o $(EXE)
	@echo "Finished cleaning."
