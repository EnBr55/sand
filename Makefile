CC=gcc

CFLAGS=-lGL -lGLU -lglut -Wall -Werror -fsanitize=address -std=gnu11 -lrt -lm

game: game.c
	$(CC) $(CFLAGS) ./game.c -o ./game
