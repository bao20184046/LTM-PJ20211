# CC=gcc
# CFLAGS=-I.
# OBJ = server.o 
# OBJ2 = client.o
# %.o: %.c
# 	$(CC) -c -o $@ $< $(CFLAGS)
# all:	server client
# server: $(OBJ)
# 	gcc -o $@ $^ $(CFLAGS)
# client: $(OBJ2)
# 	gcc -o $@ $^ $(CFLAGS)

# clean:
# 	rm *.o


CC = gcc
CFLAGS = -I.

main: main.c room.o handcard.o card.o
	$(CC) -o main main.c room.c handcard.c card.c
room.o: room.c handcard.o card.o
	$(CC) -c room.c handcard.c card.c
handcard.o: handcard.c card.o
	$(CC) -c handcard.c card.c
card.o: card.c
	$(CC) -c card.c
