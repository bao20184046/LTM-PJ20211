#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include "protocol.h"
void menu()
{
	printf("***************************************\n");
	printf("MENU:\n");
	printf("1. Create room\n");
	printf("2. Join room\n");
	printf("3. Log out\n");
	printf("***************************************\n");
	printf("Enter your choice:  ");
}

void SignIU()
{
	printf("***************************************\n");
	printf("Welcome to the world of poker\n");
	printf("1. Sign in\n");
	printf("2. Sign up\n");
	printf("***************************************\n");
	printf("Enter your choice: \n");
}


char *getNickName(char *msg)
{
	char *nickname = (char*)calloc(20,sizeof(char));
	int i = 0;
	while(msg[i+4]!='\0')
	{
		nickname[i] = msg[i+4];
		i++;
	}
	nickname[i] = '\0';
	return nickname;
}
void showListRoom(char *msg)
{
	int i = 0,j,lenght = strlen(msg);
	int id = 0,status = 0;
	char *temp = (char*)calloc(5,sizeof(char));
	printf("ID\tStatus\n");
	while(i<lenght)
	{
		j = 0;
		id = 0;
		while(msg[i]!='|'&&i<lenght)
		{
			temp[j++] = msg[i++];
		}
		j = 0;
		while(temp[j]!='-')
		{
			id*=10;
			id+=temp[j]-'0';
			j++;
		}
		j++;
		status = temp[j] - '0';
		if(id<10)
			printf(" %d\t%d\n",id,status);
		else printf("%d\t%d\n",id,status);
		i++;
	}
	free(temp);
}

int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);	
	assert(sockfd != -1 );
	char *nickname = (char*)calloc(20,sizeof(char));
	char *msg = (char*)calloc(MSG_SIZE,sizeof(char));
	int choice,isLoged_in = 0;
	int rcvsize;
	//Set Address Information
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(6666);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//Link to server
	int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	assert(res != -1);
	while(isLoged_in == 0)
	{
		SignIU();
		scanf("%d",&choice);
		while(choice<1||choice>2)
		{
			printf("Invalid selection. Re-enter:  ");
			scanf("%d",&choice);
		}
		getchar();
		switch(choice)
		{
			case 1:
			{
				strcpy(msg,makeSignInMessage());
				send(sockfd,msg,strlen(msg),0);
				rcvsize = recv(sockfd,msg,MSG_SIZE,0);
				msg[rcvsize] = '\0';
				if(msg[2] == '0' + SUCCESS_SIGNIN)
				{
					printf("Successfully sign in\n");
					strcpy(nickname,getNickName(msg));
					isLoged_in = 1;
				}
				else if(msg[2] == '0' + NOT_EXIST)
				{
					printf("Account does not exist\n");
				}
				else if(msg[2] == '0' + WRONG_PASS)
				{
					printf("Wrong password\n");
				}
				break;
			}
			case 2:
			{
				strcpy(msg,makeSignUpMessage());
				send(sockfd,msg,strlen(msg),0);
				rcvsize = recv(sockfd,msg,MSG_SIZE,0);
				msg[rcvsize] = '\0';
				if(msg[2] == '0' + SUCCESS_SIGNUP)
				{
					printf("Successfully sign up\n");
					strcpy(nickname,getNickName(msg));
					isLoged_in = 1;
				}
				else if(msg[2] == '0' + EXISTED)
				{
					printf("Already have this account\n");
				}
				break;
			}
		}
	}
	printf("nickname is %s\n",nickname );
	while(isLoged_in == 1)
	{
		menu();
		scanf("%d",&choice);
		while(choice<1||choice>3)
		{
			printf("Invalid selection. Re-enter:  ");
			scanf("%d",&choice);
		}
		switch(choice)
		{
			case 1:
			{
				strcpy(msg,makeCreateRoomMessage(nickname));
				send(sockfd,msg,strlen(msg),0);
				rcvsize = recv(sockfd,msg,MSG_SIZE,0);
				msg[rcvsize] = '\0';
				printf("%s\n", msg);
				break;
			}
			case 2:
			{
				strcpy(msg,getAvailableRoomMessage());
				send(sockfd,msg,strlen(msg),0);
				rcvsize = recv(sockfd,msg,MSG_SIZE,0);
				msg[rcvsize] = '\0';
				showListRoom(msg);
				strcpy(msg,makeJoinRoomMessage(nickname));
				if(strlen(msg)==0)
					break;
				send(sockfd,msg,strlen(msg),0);
				rcvsize = recv(sockfd,msg,MSG_SIZE,0);
				msg[rcvsize] = '\0';
				if(msg[2]=='0' + JOIN_SUCCESS)
					printf("Successfully join\n");
				if(msg[2]=='0' + FULL_SLOT)
					printf("Room is full\n");
				if(msg[2]=='0' + WRONG_RPASS)
					printf("Wrong password\n");
				if(msg[2]=='0' + ROOM_NEXIST)
					printf("Room does not exist\n");
				break;
			}
			case 3:
			{
				printf("Goodbye %s\n",nickname);
				isLoged_in = 0;
				break;
			}
		}
	}
	close(sockfd);
}