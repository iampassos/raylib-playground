CC=gcc

server:
	$(CC) src/server.c -o server.out -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

client:
	$(CC) src/client.c -o client.out -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

