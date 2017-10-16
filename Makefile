all:
	gcc -o stopwatch -O2 main.c -lGLESv2 -lglfw

install:
	cp stopwatch $(DESTDIR)/stopwatch
