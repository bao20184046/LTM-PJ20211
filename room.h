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
	int money;
	Handcard hand;
}Player;
struct box 
{
	int id;
	int status;//1 = private or  0 = public
	char password[20];
	int canPlay;
	Player player[2];//creator
	int start;//slot start
	int total;//total bet
	struct box* next;
};
typedef struct box* Room;
Room makeListRoom();
Player newPlayer(char* nickname,int money);
void joinRoom(Room headRoom, Player newPlayer);
int checkRoom(Room headRoom);
void showPlayer(Room headRoom);
Room getRoombyID(Room headRoom, int id);
void pushRoom(Room headRoom, int status, int id, char* password, Player creator_room);
int* newround(Room headRoom,int bet[],Card playercard[][7]);
void match(Room headRoom);

#endif