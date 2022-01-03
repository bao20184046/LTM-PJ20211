
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
	if(strlen(string)==0)
		return -1;
	return 1;
}
//LOGIN username password
char *makeSignInMessage()
{
	char *str1 = (char*)calloc(30,sizeof(char));
	char *str2 = (char*)calloc(20,sizeof(char));
	printf("Username: ");
	gets(str1);
	while(checkString(str1)!=1)
	{
		if(checkString(str1)==0)
			printf("Username cannot contain spaces\n");
		else printf("Username cannot be empty\n");
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
//REGISTER username password nickname
char *makeSignUpMessage()
{
	char *str1 = (char*)calloc(30,sizeof(char));
	char *str2 = (char*)calloc(20,sizeof(char));
	char *str3 = (char*)calloc(20,sizeof(char));
	printf("Username: ");
	gets(str1);
	while(checkString(str1)!=1)
	{
		if(checkString(str1)==0)
			printf("Username cannot contain spaces\n");
		else printf("Username cannot be empty\n");
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
//CREATEROOM room_type nickname password
char *makeCreateRoomMessage(char* nickname)
{
	char *password = (char*)calloc(20,sizeof(char));
	char *str = (char*)calloc(50,sizeof(char));
	int room_type,i = 4,j;
	printf("CREATE ROOM:\n");
	printf("0. Public room\n");
	printf("1. Purivate room\n");
	printf("Please enter your choice: \n");
	scanf("%d",&room_type);
	while(room_type<0||room_type>1)
	{
		printf("Invalid selection. Re-enter:  ");
		scanf("%d",&room_type);
	}
	str[0] = '0'+CREATEROOM;
	str[1] = ' ';
	str[2] = '0'+room_type;
	str[3] = ' ';
	while(i - 4 < strlen(nickname))
	{
		str[i] = nickname[i-4];
		i++;
	}
	if(room_type == 1)
	{
		getchar();
		printf("Please enter password:  " );
		gets(password);
		while(checkString(password)==0)
		{
			printf("Password cannot contain spaces\n");
			printf("Password: ");
			gets(password);
		}
		str[i++] = ' ';
		j = i;
		while(i - j < strlen(password))
		{
			str[i] = password[i - j];
			i++;
		}
	}
	str[i] = '\0';
	free(password);
	return str;
}

char *getAvailableRoomMessage()
{
	char *str = (char*)calloc(2,sizeof(char));
	str[0] = '0'+GETLIST;
	str[1]= '\0';
	return str;
}
char* makeJoinRoomMessage(char *nickname)
{
	int choice,i = 2,j;
	char *str = (char*)calloc(50,sizeof(char));
	printf("JOIN ROOM\n");
	printf("1. Join room by id\n");
	printf("2. Exit\n");
	printf("Enter your choice:  ");
	scanf("%d",&choice);
	while(choice<1||choice>2)
	{
		printf("Invalid selection. Re-enter:  ");
		scanf("%d",&choice);
	}
	if(choice==2)
	{
		strcpy(str,"\0");
		return str;
	}
	char *id = (char*)calloc(3,sizeof(char));
	char *password = (char*)calloc(20,sizeof(char));
	getchar();
	printf("Input ID:  ");
	gets(id);
	while(checkString(id)!=1)
	{
		if(checkString(id)==0)
			printf("ID cannot contain spaces\n");
		else printf("ID cannot be empty\n");
		printf("ID: ");
		gets(id);
	}
	printf("Input password: ");
	gets(password);
	while(checkString(password)==0)
	{
		printf("Password cannot contain spaces\n");
		printf("Input password: ");
		gets(password);
	}
	str[0] = '0' + JOINROOM;
	str[1] = ' ';
	while(i - 2 < strlen(nickname))
	{
		str[i] = nickname[i-2];
		i++;
	}
	str[i++] = ' ';
	j = i;
	while(i-j<strlen(id))
	{
		str[i] = id[i-j];
		i++;
	}
	if(strlen(password)!=0)
	{
		str[i++] = ' ';
		j = i;
		while(i - j < strlen(password))
		{
			str[i] = password[i-j];
			i++;
		}
	}
	str[i] = '\0';
	return str;	
}
char *makeBetMessage(char *nickname)
{
	//TODO
	
	return "alo";
}
