CC = gcc
CFLAGS = -Wall `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lSDL_ttf
LDFLAGS = `sdl-config --libs` -lSDL_ttf -lSDL_image

all: ela

ela: main.o fonction.o
	$(CC) main.o fonction.o -o ela $(LDFLAGS)

main.o: main.c fonction.h
	$(CC) $(CFLAGS) -c main.c

fonction.o: fonction.c fonction.h
	$(CC) $(CFLAGS) -c fonction.c

clean:
	rm -f *.o ela

