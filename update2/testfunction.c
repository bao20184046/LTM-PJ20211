#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "protocol.h"
#include "room.h"
Room *head = NULL;
char *listRoomMessage()
{
	int temp,i = 0;
	char *str = (char*)calloc(20,sizeof(char));
	Room *r = head;
	while(r!=NULL)
	{
		if(r->canPlay==0)
		{
			temp = r->id;
			while(temp!=0)
			{
				str[i++] = '0' + temp%10;
				temp/=10;
			}
			str[i++] = '-';
			str[i++] = '0' + r->status;
			str[i++] = '|';
		}
		r = r->next;
	}
	str[i-1] = '\0';
	return str;
}
int main(int argc, char const *argv[])
{
	
	Player p = newPlayer("ngocbao",100);
	Player q = newPlayer("bao",100);
	Player e = newPlayer("an",100);
	int i = pushRoom(&head,0,"",p);
	i = pushRoom(&head,0,"",q);
	i = pushRoom(&head,1,"abc",e);
	printf("%s\n", listRoomMessage());
	return 0;
}