libs = glib-2.0 gdk-pixbuf-2.0
build: time-to-rest.c
	mkdir -p bin
	gcc -o bin/time-to-rest time-to-rest.c -lnotify `pkg-config --libs --cflags $(libs)`
test:
	make build
	./bin/time-to-rest 2
