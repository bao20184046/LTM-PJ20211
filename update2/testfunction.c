#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "protocol.h"
#include "room.h"
Room *headRoom = NULL;
char *makeJoinNoticeToCreatorMessage(int id)
{
	int i = 2,p;
	char *str = (char*)calloc(40,sizeof(char));
	char *temp = (char*)calloc(3,sizeof(char));
	Room *r = getRoombyID(headRoom,id);
	str[0] = '0' + NOT_RES;
	str[1] = ' ';
	while(i-2 < strlen(r->player[1].nickname))
	{
		str[i] = r->player[1].nickname[i-2];
		i++;
	}
	str[i++] = ' ';
	for(int j = 0; j < 7; j++)
	{
		sprintf(temp,"%d",r->deck[j]);
		p = i;
		while(i-p<strlen(temp))
		{
			str[i] = temp[i - p];
			i++;
		}
		str[i++] = '|';
	}
	str[i-1] = '\0';
	return str;
}
char *makeJoinRoomResMessage(VALUE_RES res,int id)
{
	int i = 4,j;
	char *str = (char*)calloc(30,sizeof(char));
	char *temp = (char*)calloc(3,sizeof(char));
	Room *r = getRoombyID(headRoom,id);
	str[0] = '0' + JOI_RES;
	str[1] = ' ';
	str[2] = '0' + res;
	str[3] = ' ';
	for(int k = 2; k < 9; k++)
	{
		sprintf(temp,"%d",r->deck[k]);
		j = i;
		while(i - j < strlen(temp))
		{
			str[i]  = temp[i - j];
			i++;
		}
		str[i++] = '|';
	}
	str[i-1] = '\0';
	return str;
}
void setHandCard(char *msg)
{
	int start,end,i = 0,j=0;
	int value[7];
	while(msg[i]!=' ')
		i++;
	i++;
	while(msg[i]!=' ')
		i++;
	i++;
	while(i < strlen(msg))
	{
		value[j] = 0;
		while(msg[i]!='|'&&i<strlen(msg))
		{
			value[j]*=10;
			value[j]+=msg[i] - '0';
			i++;
		}
		printf("%d\n",value[j] );
		i++;
		j++;
	}	
}
int main(int argc, char const *argv[])
{
	Player p = newPlayer("ngocbao",100);
	Player q = newPlayer("bao",100);
	Player e = newPlayer("an",100);
	pushRoom(&headRoom,0,"",p);
	pushRoom(&headRoom,0,"",q);
	pushRoom(&headRoom,1,"abc",e);
	joinRoom(getRoombyID(headRoom,2),e);
	setDeckToRoom(getRoombyID(headRoom,2));
	printf("%s\n", makeJoinNoticeToCreatorMessage(2));
	setHandCard(makeJoinRoomResMessage(JOIN_SUCCESS,2));
	printf("%s\n", makeJoinRoomResMessage(JOIN_SUCCESS,2));
	return 0;
}