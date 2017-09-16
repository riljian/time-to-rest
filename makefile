libs = glib-2.0 gdk-pixbuf-2.0
build: time-to-rest.c
	gcc -o bin/time-to-rest time-to-rest.c -lnotify `pkg-config --libs --cflags $(libs)`
