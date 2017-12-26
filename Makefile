.PHONY: all clean install uninstall

CC=gcc
CFLAGS=-O2
LDFLAGS=-s
LDLIBS=-lglfw -lGLESv2

SRC=main.c
ICON=stopwatch.png
DESKTOP=stopwatch.desktop
BIN=stopwatch

BINDST=/usr/bin/stopwatch
ICONDST=/usr/share/icons/hicolor/16x16/apps/stopwatch.png \
        /usr/share/pixmaps/stopwatch.png
DESKTOPDST=/usr/share/applications/stopwatch.desktop

all:
	$(CC) $(CFLAGS) -o $(BIN) $(LDFLAGS) $(SRC) $(LDLIBS)

clean:
	rm -f $(BIN)

install:
	for i in $(BINDST); do cp $(BIN) $$i; done
	for i in $(ICONDST); do cp $(ICON) $$i; done
	for i in $(DESKTOPDST); do cp $(DESKTOP) $$i; done

uninstall:
	rm -f $(BINDST) $(ICONDST) $(DESKTOPDST)
