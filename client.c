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
	printf("Enter your choice\n");
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
	// while(1)
	// {
	// 	menu();
	// 	char buff[128] = {0};
	// 	printf("Please Input:");
	// 	fgets(buff,128,stdin);
	// 	if(strncmp(buff,"end",3) ==0 )
	// 	{
	// 		break;
	// 	}
	// 	send(sockfd,buff,strlen(buff),0);
	// 	memset(buff,0,128);
	// 	recv(sockfd,buff,127,0);
	// 	printf("RecvBuff:%s\n",buff);
 //        printf("\n");
	// }
	close(sockfd);
}