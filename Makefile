CC=gcc

all:
	$(CC) src/game.c -o game.out -pthread -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm game.out

