.PHONY: all clean install uninstall

CC=gcc
CFLAGS=-O2
LDFLAGS=-s
LDLIBS=-lglfw -lGLESv2

SRC=main.c
DST=stopwatch

BINDIR=/usr/bin

all:
	$(CC) $(CFLAGS) -o $(DST) $(LDFLAGS) $(SRC) $(LDLIBS)

clean:
	rm -f $(DST)

install:
	cp $(DST) $(BINDIR)/$(DST)

uninstall:
	rm -f $(BINDIR)/$(DST)
