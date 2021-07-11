CC=gcc

CFLAGS=-lGL -lGLU -lglut -Wall -Werror -fsanitize=address -std=gnu11 -lrt -lm

game: src/game.c src/renderer.c src/sand.c src/cells.c
	$(CC) $(CFLAGS) $^ -o ./build/game

test: src/game.c src/renderer.c src/sand.c src/cells.c
	$(CC) $(CFLAGS) $^ -o ./build/game
	./game
