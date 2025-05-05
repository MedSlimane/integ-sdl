ex: main.o fonctions.o
	gcc main.o fonctions.o -o ex -g -lSDL -lSDL_image -lX11 -lSDL_gfx

main.o: main.c fonctions.h
	gcc -c main.c -g -I/usr/include/SDL

fonctions.o: fonctions.c fonctions.h
	gcc -c fonctions.c -g -I/usr/include/SDL -I/usr/include/SDL_gfx

clean:
	rm -f ex main.o fonctions.o
