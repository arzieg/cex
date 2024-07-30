CC=gcc
#CFLAGS=-lncurses -ggdb -Wall -ftrapv
#CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g 
CFLAGS=-Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wformat -O0 -g 
LFLAGS=-lncurses
SRC=src
OBJ=obj
#SRCS=$(wildcard $(SRC)/*.c)
#OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BINDIR=bin
#BIN=$(BINDIR)/tcp_server_toupper


syncclient: SRCS=$(SRC)/syncclient.c
syncclient: OBJS=$(OBJ)/syncclient.o
syncclient: BIN=$(BINDIR)/syncclient
syncclient: clean
syncclient:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

cur2: SRCS=$(SRC)/cur2.c
cur2: OBJS=$(OBJ)/cur2.o
cur2: BIN=$(BINDIR)/cur2
cur2: clean
cur2:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

cur3: SRCS=$(SRC)/cur3.c
cur3: OBJS=$(OBJ)/cur3.o
cur3: BIN=$(BINDIR)/cur3
cur3: clean
cur3:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

cur4: SRCS=$(SRC)/cur4.c
cur4: OBJS=$(OBJ)/cur4.o
cur4: BIN=$(BINDIR)/cur4
cur4: clean
cur4:
	$(CC) $(CFLAGS) -c $(SRCS) -o $(OBJS) $(LFLAGS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BINDIR)/* $(OBJ)/*
	@echo "Finished cleaning."
