#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "protocol.h"
char *makeJoinNoticeToCreatorMessage(int position)
{
	int i = 2;
	char *str = (char*)calloc(6,sizeof(char));
	char p[3];
	memset(p,0,sizeof(p));
	sprintf(p,"%d",position);
	str[0] = '0' + NOT_RES;
	str[1] = ' ';
	while(i-2 < strlen(p))
	{
		str[i] = p[i-2];
		i++;
	}
	str[i] = '\0';
	return str;
}
int main(int argc, char const *argv[])
{
	
	printf("%s\n", makeJoinNoticeToCreatorMessage(123));
	return 0;
}