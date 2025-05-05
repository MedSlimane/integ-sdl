CC = gcc
CFLAGS = -g -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
SRC = main.c fonction.c
OBJ = $(SRC:.c=.o)

prog: $(OBJ)
	$(CC) $(OBJ) -o prog $(CFLAGS)

clean:
	rm -f $(OBJ) prog
