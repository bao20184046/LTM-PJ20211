#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include "protocol.h"
#include "card.h"
Card card[7];
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

void setHandCard(char *msg)
{
	memset(card,0,sizeof(card));
	int start,i = 0,j=0;
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
		i++;
		j++;
	}
	i = 0;
	if(msg[0] == '0' + NOT_RES)
	{
		while(i < 5)
		{
			card[i] = newcard(value[i+2]);
			i++;
		}
		card[5] = newcard(value[0]);
		card[6] = newcard(value[1]);
	}
	else
	{
		while(i < 7)
		{
			card[i] = newcard(value[i]);
			i++;
		}
	}	
}
void drawTable(int round)
{
	if(round==1)
	{
		printf("------------------------------------------\n");
		printf("|\t| | \t| | \t| | \t| | \t|\n");
		printf("|\t| | \t| | \t| | \t| | \t|\n");
		printf("------------------------------------------\n");
	}
	if(round==2)
	{
		printf("------------------------------------------\n");
		printf("|%s\t| |%s\t| |%s\t| | \t| | \t|\n",card[0].showvalue,card[1].showvalue,card[2].showvalue);
		printf("|%s\t| |%s\t| |%s\t| | \t| | \t|\n",card[0].showtype,card[1].showtype,card[2].showtype);
		printf("------------------------------------------\n");
	}
	if(round==3)
	{
		printf("------------------------------------------\n");
		printf("|%s\t| |%s\t| |%s\t| |%s\t| | \t|\n",card[0].showvalue,card[1].showvalue,card[2].showvalue,card[3].showvalue);
		printf("|%s\t| |%s\t| |%s\t| |%s\t| | \t|\nn",card[0].showtype,card[1].showtype,card[2].showtype,card[3].showtype);
		printf("------------------------------------------\n");
	}
	if(round==4)
	{
		printf("------------------------------------------\n");
		printf("|%s\t| |%s\t| |%s\t| |%s\t| |%s\t|\n",card[0].showvalue,card[1].showvalue,card[2].showvalue,card[3].showvalue,card[4].showvalue);
		printf("|%s\t| |%s\t| |%s\t| |%s\t| |%s\t|\n",card[0].showtype,card[1].showtype,card[2].showtype,card[3].showtype,card[4].showtype);
		printf("------------------------------------------\n");
	}
}
void drawHand()
{
	printf("------------------\n");
	printf("|%s\t| |%s\t|\n",card[5].showvalue,card[6].showvalue);
	printf("|%s\t| |%s\t|\n",card[5].showtype,card[6].showtype);
	printf("------------------\n");
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
void showIDCreatedRoom(char *msg)
{
	int i = 2;
	printf("Successful room creation. Room ID is ");
	while(i < strlen(msg))
	{
		printf("%c", msg[i]);
		i++;
	}
	printf("\n");
}
void configOpponent(char *msg,char *opponent)
{
	int i = 0;
	while(msg[i+2]!=' ')
	{
		opponent[i] = msg[i+2];
		i++;
	}
	opponent[i] = '\0';
	printf("**************************************\n");
	printf("%s join your room. Battle start!!!!\n",opponent );
}
int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);	
	assert(sockfd != -1 );
	char *nickname = (char*)calloc(20,sizeof(char));
	char *opponent = (char*)calloc(20,sizeof(char));
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
	printf("Login successfully. Welcome %s\n",nickname );
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
				showIDCreatedRoom(msg);
				printf("Please wait for another player to enter the room.\n");
				rcvsize = recv(sockfd,msg,MSG_SIZE,0);
				printf("%d\n",rcvsize );
				msg[rcvsize] = '\0';
				printf("%s\n",msg );
				configOpponent(msg,opponent);
				setHandCard(msg);
				drawTable(4);
				drawHand();
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
				{
					printf("Successfully join\n");
					printf("%s\n",msg );
					setHandCard(msg);
					drawTable(4);
					drawHand();
				}
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