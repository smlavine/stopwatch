.PHONY: all clean install uninstall

CC=cc
CFLAGS=-O2
LDFLAGS=-s
LDLIBS=-lm

SRC=main.c
BIN=stopwatch

BINDST=/usr/bin/stopwatch

all:
	$(CC) $(CFLAGS) -o $(BIN) $(LDFLAGS) $(SRC) $(LDLIBS)

clean:
	rm -f $(BIN)

install:
	cp $(BIN) $(BINDST)

uninstall:
	rm -f $(BINDST)
