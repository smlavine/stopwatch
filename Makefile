.PHONY: all clean install uninstall flatpak flatpak-clean flatpak-install flatpak-uninstall

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

# For flatpak creation
LIBGLFW_PATH=/usr/lib/x86_64-linux-gnu/libglfw.so.3.2
LIBGLES2_PATH=/usr/lib/x86_64-linux-gnu/mesa-egl/libGLESv2.so.2.0.0
LIBGLAPI_PATH=/usr/lib/x86_64-linux-gnu/libglapi.so.0.0.0

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

flatpak: flatpak-clean
	mkdir -p stopwatch-flatpak/files/bin
	cp metadata stopwatch-flatpak/metadata
	cp $(BIN) stopwatch-flatpak/files/bin/stopwatch
	mkdir -p stopwatch-flatpak/files/lib
	cp $(LIBGLFW_PATH) stopwatch-flatpak/files/lib/libglfw.so.3
	cp $(LIBGLES2_PATH) stopwatch-flatpak/files/lib/libGLESv2.so
	cp $(LIBGLAPI_PATH) stopwatch-flatpak/files/lib/libglapi.so
	mkdir -p stopwatch-flatpak/export/share/icons/hicolor/16x16/apps
	cp $(ICON) stopwatch-flatpak/export/share/icons/hicolor/16x16/apps/org.markweston.stopwatch.png
	mkdir -p stopwatch-flatpak/export/share/applications
	cp $(DESKTOP) stopwatch-flatpak/export/share/applications/org.markweston.stopwatch.desktop
	sed -i 's/Icon=stopwatch/Icon=org.markweston.stopwatch/g' stopwatch-flatpak/export/share/applications/org.markweston.stopwatch.desktop
	flatpak build-export flatpak stopwatch-flatpak
	flatpak build-bundle flatpak stopwatch.flatpak org.markweston.stopwatch
	rm -rf flatpak stopwatch-flatpak

flatpak-clean:
	rm -f stopwatch.flatpak

flatpak-install:
	flatpak install --bundle stopwatch.flatpak

flatpak-uninstall:
	flatpak uninstall org.markweston.stopwatch
