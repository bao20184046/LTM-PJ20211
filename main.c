#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "room.h"
Room headRoom = NULL;
int main(int argc, char const *argv[])
{
	Player p1 = newPlayer("Bao",40000);
	Player p2 = newPlayer("Du",40000);
	headRoom = makeListRoom();
	pushRoom(headRoom,0,123,"",p1);
	joinRoom(headRoom,p2);
	int _continue = 0;
	while(_continue==0)
	{
		match(getRoombyID(headRoom,123));
		showPlayer(headRoom);
		printf("--------------------------------------------------\n");	
		_continue = checkRoom(headRoom);
	}
	if(_continue==1)
		printf("Winner is %s\n",headRoom->player[0].nickname );
	if(_continue==2)
		printf("Winner is %s\n",headRoom->player[1].nickname );
	return 0;
}