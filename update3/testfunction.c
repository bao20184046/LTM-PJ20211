#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "protocol.h"
// #include "room.h"
// Room *headRoom = NULL;
char *makeBetMessage(int newbet)
{
	int i = 2;
	char *str = (char*)calloc(30,sizeof(char));
	char *temp = (char*)calloc(4,sizeof(char));
	str[0] = '0' + 5;
	str[1] = ' ';
	sprintf(temp,"%d",newbet);
	while(i-2 < strlen(temp))
	{
		str[i] = temp[i-2];
		i++;
	}
	str[i] = '\0';
	return str;
}


int main(int argc, char const *argv[])
{
	printf("%s\n", makeBetMessage(-55));
	return 0;
}