CFLAGS = -w -O2 -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
CC = gcc

# Target executable
TARGET = jeu.x

# Object files
OBJS = main.o fonction.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)

main.o: main.c fonction.h
	$(CC) -c main.c -o $@

fonction.o: fonction.c fonction.h
	$(CC) -c fonction.c -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

