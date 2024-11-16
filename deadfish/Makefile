#
# Compiler flags
#
CC     = gcc
FFLAGS = -fdiagnostics-show-caret -fdiagnostics-show-option
CFLAGS = -Wall -Werror 
LFLAGS = -L/usr/lib/x86_64-linux-gnu -lssl -lcrypto 

#
# Project files
#
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
BIN=deadfish


#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(BIN)
DBGOBJS = $(patsubst $(SRC)/%c, $(DBGDIR)/%o, $(SRCS))
DBGCFLAGS = -Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(BIN)
RELOBJS = $(patsubst $(SRC)/%c, $(RELDIR)/%o, $(SRCS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXE)

$(info $$DBGEXE = $(DBGEXE))
$(info $$DBGOBJS = $(DBGOBJS))

$(DBGEXE): $(DBGOBJS)
	$(CC) $(FFLAGS) $(CFLAGS) $(DBGCFLAGS) $(DBGOBJS) -o $@ $(LFLAGS)

$(DBGDIR)/%.o: $(SRC)/%.c
	$(CC) $(FFLAGS) $(CFLAGS) $(DBGCFLAGS) -c $< -o $@ $(LFLAGS)

#
# Release rules
#
release: $(RELEXE)

$(info $$RELEXE = $(RELEXE))
$(info $$RELOBJS = $(RELOBJS))

$(RELEXE): $(RELOBJS)
	$(CC) $(FFLAGS) $(CFLAGS) $(RELCFLAGS) $(RELOBJS) -o $@ $(LFLAGS)

$(RELDIR)/%.o: $(SRC)/%.c
	$(CC) $(FFLAGS) $(CFLAGS) $(RELCFLAGS) -c $< -o $@ $(LFLAGS)
		

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	@echo "Cleaning..."
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
	@echo "Finished cleaning."
	
