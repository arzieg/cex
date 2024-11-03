#
# Compiler flags
#
CC     = gcc
FFLAGS = -fdiagnostics-show-caret -fdiagnostics-show-option
CFLAGS = -Wall -Werror -Wextra
LFLAGS = -L/usr/lib/x86_64-linux-gnu -lssl -lcrypto 

#
# Project files
#
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%c, $(OBJ)/%o, $(SRCS))
BIN=cryptme


#
# Debug build settings
#
DBGDIR = debug
DBGEXE = $(DBGDIR)/$(BIN)
DBGOBJS = $(addprefix $(OBJ)/, $(OBJS))
DBGCFLAGS = -Wpedantic -Wall -Wextra -Werror -std=gnu17 -Wstrict-prototypes  -Wmissing-prototypes -Wmisleading-indentation -Wduplicated-cond -Wold-style-definition -Wconversion -Wshadow -Winit-self -Wfloat-equal -Wwrite-strings -Wcast-align=strict -Wformat -O0 -g

#
# Release build settings
#
RELDIR = release
RELEXE = $(RELDIR)/$(BIN)
#RELOBJS = $(addprefix $(OBJ)/, $(OBJS))
RELOBJS = $(OBJS)
$(info $$RELOBJS = $(RELOBJS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
#$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^
	$(CC) $(FFLAGS) $(CFLAGS) $(DBGCFLAGS) $(OBJS) -o $@ $(LFLAGS)

$(DBGDIR)/%.o: $(SRC)/%.c
#$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<
	$(CC) $(FFLAGS) $(CFLAGS) $(DBGCFLAGS) -c $< -o $@ $(LFLAGS)

#
# Release rules
#
release: $(RELEXE)
$(info $$RELEXE = $(RELEXE))
$(info $$RELOBJS = $(RELOBJS))

$(RELEXE): $(RELOBJS)
#$(CC) $(FFLAGS) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^
	$(CC) $(FFLAGS) $(CFLAGS) $(RELCFLAGS) $(RELOBJS) -o $@ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
#$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<
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
	