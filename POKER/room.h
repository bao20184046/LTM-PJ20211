#ifndef __ROOM__
#define __ROOM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "handcard.h"
#include "card.h"


typedef struct box 
{
	int id;
	int status;//1 = private or  0 = public
	char password[20];
	int canPlay;
	char player1[20];
	char player2[20];
	int deck[9];
	struct box* next;
}Room;
Room *makeListRoom();
void joinRoom(Room *headRoom,char* joiner);
Room *getRoombyID(Room *headRoom, int id);
int pushRoom(Room **headRoom, int status, char* password, char *cretor);
int* newround(Room *headRoom,int bet[],Card playercard[][7]);
void setDeckToRoom(Room *room);
void removeRoom(Room **headRoom,int id);
#endif