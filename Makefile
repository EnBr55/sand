CC=gcc

CFLAGS=-lGL -lGLU -lglut -Wall -Werror -fsanitize=address -std=gnu11 -lrt -lm

game: game.c renderer.c sand.c cells.c
	$(CC) $(CFLAGS) $^ -o ./game

test: game.c renderer.c sand.c cells.c
	$(CC) $(CFLAGS) $^ -o ./game
	./game
