CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: client server 

server: server.o protocol.o user.o card.o handcard.o room.o
	${CC} protocol.o user.o card.o handcard.o room.o server.o -o server

server.o: server.c
	${CC} ${CFLAGS} server.c

client: client.o protocol.o card.o handcard.o
	${CC} protocol.o card.o handcard.o client.o -o client

client.o: client.c 
	${CC} ${CFLAGS} client.c

protocol.o: protocol.c
	${CC} ${CFLAGS} protocol.c

card.o: card.c
	${CC} ${CFLAGS} card.c

handcard.o: handcard.c
	${CC} ${CFLAGS} handcard.c

user.o: user.c
	${CC} ${CFLAGS} user.c

room.o: room.c
	${CC} ${CFLAGS} room.c
clean:
	rm -f *.o *~
