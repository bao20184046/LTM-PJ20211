#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct box 
{
	int id;
	struct box* next;
}Room;

Room *makeListRoom()
{
	Room *new = (Room*)calloc(1,sizeof(struct box));
	memset(new,0,sizeof(struct box));
	return new;
}
Room *newRoom()
{
	Room *new = (Room*)malloc(sizeof(Room));
	new->next = NULL;
	return new;
}
int pushRoom(Room **headRoom)
{
	int id = 0;
	Room *new = newRoom();
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
void removeRoom(int id,Room *headRoom)
{
	Room *r = getRoombyID(headRoom,id);
	if(r == NULL)
	{
		return;
	}
	if(r == headRoom)
	{
		headRoom = headRoom->next;
	}
	else
	{
		Room *r1 = headRoom;
		while(r1->next != r)
		{
			r1 = r1->next;
		}
		r1->next = r->next;
	}
}
void travel(Room *room)
{
	Room *r = room;
	while(r!=NULL)
	{
		printf("%d\n",r->id );
		r = r->next;
	}
}
int main(int argc, char const *argv[])
{
	Room *headRoom = makeListRoom();
	pushRoom(&headRoom);
	pushRoom(&headRoom);
	pushRoom(&headRoom);
	travel(headRoom);
	removeRoom(1,headRoom);
	travel(headRoom);
	return 0;
}


