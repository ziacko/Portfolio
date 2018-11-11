all: lib/keyboard.o lib/joystick.o lib/glhelper.o lib/timer.o lib/misc.o
	g++ main.cc lib/keyboard.o lib/joystick.o lib/glhelper.o lib/timer.o lib/misc.o -o bin/main -L/usr/lib `sdl-config --cflags --libs` -lSDL_ttf -lSDL_image `pkg-config opencv --cflags --libs` -lGL -lGLU -lGLEW -lglut -lvlc -lpthread

lib/keyboard.o: src/keyboard.h src/keyboard.cc
	g++ src/keyboard.cc -c -o lib/keyboard.o

lib/joystick.o: src/joystick.h src/joystick.cc
	g++ src/joystick.cc -c -o lib/joystick.o

lib/glhelper.o: src/glhelper.h src/glhelper.cc
	g++ src/glhelper.cc -c -o lib/glhelper.o

lib/timer.o: src/timer.h src/timer.cc
	g++ src/timer.cc -c -o lib/timer.o

lib/misc.o: src/misc.h src/misc.cc
	g++ src/misc.cc -c -o lib/misc.o

clean:
	@rm -f *~ src/*~ lib/* bin/*
