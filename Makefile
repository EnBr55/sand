CC=gcc

CFLAGS=-lGL -lGLU -lglut

#.PHONY: game

game: game.c
	$(CC) $(CFLAGS) ./game.c -o ./game
