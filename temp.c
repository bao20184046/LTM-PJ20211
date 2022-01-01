#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
char *getLoginMessage(char* str1,char* str2)
{
	int total_length = strlen(str1) + strlen(str2) + 2;
	char *str = (char*)calloc(total_length,sizeof(char));
	int i = 0;
	while(i < strlen(str1))
	{
		str[i] = str1[i];
		i++;
	}
	str[i] = ' ';
	i++;
	while(i < total_length - 1)
	{
		str[i] = str2[i-strlen(str1)-1];
		i++;
	}
	str[i] = '\0';
	return str;
}
int main(int argc, char const *argv[])
{
	printf("%ld - %s\n",strlen(getLoginMessage("ngocbao","langle2k")),getLoginMessage("ngocbao","langle2k"));
	return 0;
}