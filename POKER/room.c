#include "room.h"
#include "handcard.h"
#include "card.h"
Room *makeListRoom()
{
	Room *new = (Room*)calloc(1,sizeof(struct box));
	memset(new,0,sizeof(struct box));
	return new;
}


void joinRoom(Room *room, char* joiner)
{
	if(room->canPlay==1)
	{
		return;
	}
	strcpy(room->player2,joiner);
	room->canPlay = 1;
}

Room *getRoombyID(Room *headRoom, int id)
{
	Room *r = headRoom;
	while(r!=NULL)
	{
		if(r->id == id)
			return r;
		r = r->next;
	}
	return NULL;
}
void setDeckToRoom(Room *room)
{
	memset(room->deck,0,sizeof(room->deck));
	int * deck = rollcard();
	for(int i = 0; i < 9; i++)
	{
		room->deck[i] = deck[i];
	}
}



Room *newRoom(int status, char* password, char* creator)
{
	Room *new = (Room*)malloc(sizeof(Room));
	memset(new,0,sizeof(struct box));
	new->status = status;
	new->canPlay = 0;
	strcpy(new->password,password);
	strcpy(new->player1,creator);
	new->next = NULL;
	return new;
}
int pushRoom(Room **headRoom, int status, char* password, char* creator)
{
	int id = 0;
	Room *new = newRoom(status,password,creator);
	if((*headRoom)->id == 0)
	{
		*headRoom = new;
		(*headRoom)->id = 1;
		return 1;
	}
	Room *r = *headRoom;
	while(r->next!=NULL)
	{
		r = r->next;
	}
	r->next = new;
	id = r->id + 1;
	r->next->id = id;
	return id;
}

void removeRoom(Room **headRoom, int id)
{
	Room *r = getRoombyID(*headRoom,id);
	if(r == NULL)
	{
		return;
	}
	if(r == *headRoom)
	{
		*headRoom = (*headRoom)->next;
		if(*headRoom == NULL)
			*headRoom = makeListRoom();
	}
	else
	{
		Room *r1 = *headRoom;
		while(r1->next != r)
		{
			r1 = r1->next;
		}
		r1->next = r->next;
	}
}
