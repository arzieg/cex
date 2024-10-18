CC=gcc
#CFLAGS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/lib64/glib-2.0/include -ggdb -Wall -ftrapv
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g
LFLAGS=-L/usr/lib/x86_64-linux-gnu -lssl -lcrypto 
SRC=src
OBJ=obj
PRG1=ssl_aes256
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BINDIR=bin
BIN1=$(BINDIR)/$(PRG1) 

all: $(OBJS) $(BIN1) $(BIN2) 

$(BIN1): $(OBJ)/$(PRG1).o
	$(CC) $(CFLAGS) $(OBJ)/$(PRG1).o -o $@ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LFLAGS)


#test: BIN=$(BINDIR)/filedir
#test: clean
#test: 
#	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
#	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)


.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BINDIR)/* $(OBJ)/*
	@echo "Finished cleaning."
