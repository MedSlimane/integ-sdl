CC = gcc
CFLAGS = -Wall -Wextra -g
SDLFLAGS = `sdl-config --cflags --libs` -lSDL_image -lSDL_ttf -lSDL_mixer -lm

SRC = main.c integrated.c menu.c score.c
OBJ = $(SRC:.c=.o)
EXE = game

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $@ $(SDLFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXE)

.PHONY: all clean