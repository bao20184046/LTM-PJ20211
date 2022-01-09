
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
	strcpy(str2,getpass("Password: "));
	while(checkString(str2)==0)
	{
		printf("Password cannot contain spaces\n");
		strcpy(str2,getpass("Password: "));
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
	strcpy(str2,getpass("Password: "));
	while(checkString(str2)==0)
	{
		printf("Password cannot contain spaces\n");
		strcpy(str2,getpass("Password: "));
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
	printf("1. Private room\n");
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
		strcpy(password,getpass("Password: "));
		while(checkString(password)==0)
		{
			printf("Password cannot contain spaces\n");
			strcpy(password,getpass("Password: "));
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
	strcpy(password,getpass("Password: "));
	while(checkString(password)==0)
	{
		printf("Password cannot contain spaces\n");
		strcpy(password,getpass("Password: "));
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
//5 bet
char *makeBetMessage(int newbet)
{
	int i = 2;
	char *str = (char*)calloc(7,sizeof(char));
	char *temp = (char*)calloc(4,sizeof(char));
	str[0] = '0' + RAISE;
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
char *makeNewRoundBetMessage(int newbet)
{
	int i = 2;
	char *str = (char*)calloc(7,sizeof(char));
	char *temp = (char*)calloc(4,sizeof(char));
	str[0] = '0' + RAISE;
	str[1] = '+';
	sprintf(temp,"%d",newbet);
	while(i-2 < strlen(temp))
	{
		str[i] = temp[i-2];
		i++;
	}
	str[i] = '\0';
	return str;
}
char *makeEndMessage(ENDGAME_TYPE type)
{
	char *str = (char*)calloc(5,sizeof(char));
	str[0] = '0' + ENDGAME;
	str[1] = ' ';
	str[2] = '0' + type;
	str[3] = '\0';
	return str;
}
char *getScoreMessage(char *nickname)
{
	int i = 2;
	char *str = (char*)calloc(5,sizeof(char));
	str[0] = '0' + GETSCORE;
	str[1] = ' ';
	while(i-2 < strlen(nickname))
	{
		str[i] = nickname[i-2];
		i++;
	}
	str[i] = '\0';
	return str;
}
char *makePlusScoreMessage(char *nickname,int score)
{
	int i = 2,j;
	char *str = (char*)calloc(5,sizeof(char));
	char *temp = (char*)calloc(4,sizeof(char));
	sprintf(temp,"%d",score);
	str[0] = '0' + PLUS;
	str[1] = ' ';
	while(i-2 < strlen(nickname))
	{
		str[i] = nickname[i-2];
		i++;
	}
	str[i++]= ' ';
	j = i;
	while(i - j < strlen(temp))
	{
		str[i] = temp[i - j];
		i++;
	}
	str[i] = '\0';
	return str;
}