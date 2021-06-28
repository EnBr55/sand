CC=gcc

CFLAGS=-lGL -lGLU -lglut -Wall -Werror -fsanitize=address -std=gnu11 -lrt -lm

game: game.c renderer.c
	$(CC) $(CFLAGS) $^ -o ./game
