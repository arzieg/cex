CC=gcc
CFLAGS=-ggdb -Wall -ftrapv  
EXE=debugexample

all: $(EXE)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)


.PHONY: clean
clean:
	@echo "Cleaning..."
	-rm -f *.dSYM *.o $(EXE)
	@echo "Finished cleaning."
