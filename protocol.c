
#include "protocol.h"
int checkString(char* string)
{
	int i = 0;
	while(string[i]!='\0')
	{
		if(string[i] == ' ')
			return 0;
		i++;
	}
	return 1;
}
char *makeSignInMessage()
{
	char *str1 = (char*)calloc(30,sizeof(char));
	char *str2 = (char*)calloc(20,sizeof(char));
	printf("Username: ");
	gets(str1);
	while(checkString(str1)==0)
	{
		printf("Username cannot contain spaces\n");
		printf("Username: ");
		gets(str1);
	}
	printf("Password: ");
	gets(str2);
	while(checkString(str2)==0)
	{
		printf("Password cannot contain spaces\n");
		printf("Password: ");
		gets(str2);
	}
	int total_length = strlen(str1) + strlen(str2) + 3;
	char *str = (char*)calloc(total_length,sizeof(char));
	int i = 1;
	str[0] = '0' + LOGIN;
	while(i < strlen(str1) + 1)
	{
		str[i] = str1[i-1];
		i++;
	}
	str[i] = ' ';
	i++;
	while(i < total_length - 1)
	{
		str[i] = str2[i-strlen(str1)-2];
		i++;
	}
	str[i] = '\0';
	free(str1);
	free(str2);
	return str;
}
char *makeSignUpMessage()
{
	char *str1 = (char*)calloc(30,sizeof(char));
	char *str2 = (char*)calloc(20,sizeof(char));
	char *str3 = (char*)calloc(20,sizeof(char));
	printf("Username: ");
	gets(str1);
	while(checkString(str1)==0)
	{
		printf("Username cannot contain spaces\n");
		printf("Username: ");
		gets(str1);
	}
	printf("Password: ");
	gets(str2);
	while(checkString(str2)==0)
	{
		printf("Password cannot contain spaces\n");
		printf("Password: ");
		gets(str2);
	}
	printf("Nickname: ");
	gets(str3);
	while(checkString(str3)==0)
	{
		printf("Nickname cannot contain spaces\n");
		printf("Nickname: ");
		gets(str3);
	}
	int total_length = strlen(str1) + strlen(str2) + strlen(str3) + 4;
	char *str = (char*)calloc(total_length,sizeof(char));
	int i = 1,j = 0;
	str[0] = '0' + REGISTER;
	while(j < strlen(str1))
	{
		str[i] = str1[j];
		i++;
		j++;
	}
	str[i] = ' ';
	i++;
	j = 0;
	while(j < strlen(str2))
	{
		str[i] = str2[j];
		i++;
		j++;
	}
	str[i] = ' ';
	i++;
	j = 0;
	while(j < strlen(str3))
	{
		str[i] = str3[j];
		i++;
		j++;
	}
	str[i] = '\0';
	free(str1);
	free(str2);
	return str;
}
