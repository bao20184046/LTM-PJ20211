#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "room.h"
#include "handcard.h"
#include "card.h"
Room headRoom = NULL;
int main(int argc, char const *argv[])
{
	int id;
	Player p1 = newPlayer("Bao",40000);
	Player p2 = newPlayer("Du",40000);
	headRoom = makeListRoom();
	pushRoom(headRoom,0,123,"",p1);
	printf("Input ID room:\n");
	scanf("%d",&id);
	Room r = getRoombyID(headRoom,123);
	joinRoom(r,p2);
	printf("%s\n",r->player[1].nickname );
	int _continue = 0;
	while(_continue==0)
	{
		match(r);
		showPlayer(r);
		printf("--------------------------------------------------\n");	
		_continue = checkRoom(r);
	}
	if(_continue==1)
		printf("Winner is %s\n",r->player[0].nickname );
	if(_continue==2)
		printf("Winner is %s\n",r->player[1].nickname );
	return 0;
}