#ifndef __ROOM__
#define __ROOM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "handcard.h"
#include "card.h"
typedef struct
{
	char nickname[20];
	int chip;
	Handcard hand;
}Player;

typedef struct box 
{
	int id;
	int status;//1 = private or  0 = public
	char password[20];
	int canPlay;
	Player player[2];//creator
	int start;//slot start
	int bet[2];
	int deck[9];
	struct box* next;
}Room;
Room *makeListRoom();
Player newPlayer(char* nickname,int money);
void joinRoom(Room *headRoom, Player newPlayer);
int checkRoom(Room *headRoom);
void showPlayer(Room *headRoom);
Room *getRoombyID(Room *headRoom, int id);
int pushRoom(Room **headRoom, int status, char* password, Player creator_room);
int* newround(Room *headRoom,int bet[],Card playercard[][7]);
// void match(Room *headRoom);
void setDeckToRoom(Room *room);
void removeRoom(Room *headRoom,int id);
#endif