prog:mainES.o fonctionES.o 
	gcc mainES.o fonctionES.o  -o prog -lSDL -lSDL_ttf -g -lSDL_image -lSDL_mixer -g -lm
main.o:mainES.c
	gcc -c mainES.c -g

fonctionES.o:fonctionES.c
	gcc -c fonctionES.c -g

clean:
	rm -f *.o prog

