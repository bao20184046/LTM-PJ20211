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
#include "handcard.h"
Card card[7];
Card opponentcard[7];
int bet[2];
int Round;
int chip;
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
			if(msg[i]==' ')
				break;
			value[j]*=10;
			value[j]+=msg[i] - '0';
			i++;
		}
		if(j==6)
			break;
		i++;
		j++;
	}
	i = 0;
	if(msg[0] == '0' + NOT_RES)
	{
		while(i < 5)
		{
			card[i] = newcard(value[i+2]);
			opponentcard[i] = newcard(value[i+2]);
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
			if(i < 5)
			{
				opponentcard[i] = newcard(value[i]);
			}
			i++;
		}
	}	
}
void setPlay()
{
	memset(card,0,sizeof(card));
	memset(opponentcard,0,sizeof(opponentcard));
	Round = ROUND;
	bet[0] = BET;
	bet[1] = BET;
	chip = CHIP;
}
void drawTable(int round)
{
	printf("****************************************\n");
	printf("Round %d\n",round );
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
	printf("****************************************\n");
}
void drawHand()
{
	printf("------------------\n");
	printf("|%s\t| |%s\t|\n",card[5].showvalue,card[6].showvalue);
	printf("|%s\t| |%s\t|\n",card[5].showtype,card[6].showtype);
	printf("------------------\n");
}
void drawOpponentHand()
{
	printf("------------------\n");
	printf("|%s\t| |%s\t|\n",opponentcard[5].showvalue,opponentcard[6].showvalue);
	printf("|%s\t| |%s\t|\n",opponentcard[5].showtype,opponentcard[6].showtype);
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
}
void configOpponent2(char *msg,char *opponent)
{
	int i = 0,j;
	while(msg[i]!=' ')
		i++;
	i++;
	while(msg[i]!=' ')
		i++;
	i++;
	while(msg[i]!=' ')
		i++;
	i++;
	j = i;
	while(i < strlen(msg))
	{
		opponent[i - j] = msg[i];
		i++;
	}
	opponent[i - j] = '\0';
}
//ENDGAME playerwin la1 la2
//RAISE bet
int* processOpponentAction(char* msg)
{
	int i = 2;
	int isNegative = 0;
	int *result = (int*)calloc(4,sizeof(int));
	memset(result,0,sizeof(result)); 
	if(msg[0] == '0' + END_RES)
	{
		printf("%s\n", msg);
		result[0] = ENDGAME;
		if(msg[2] == '-')
		{
			result[1] = -1;
			return result;
		}
		result[1] = msg[2] - '0';
		i = 4;
		while(msg[i]!=' ')
		{
			result[2] *=10;
			result[2] +=msg[i] - '0';
			i++;
		}
		i++;
		while(i < strlen(msg))
		{
			result[3] *=10;
			result[3] +=msg[i] - '0';
			i++;
		}
		printf("%d\n",result[3] );
	}
	else
	{
		if(msg[1] == '+')
			Round++;
		if(msg[2] == '-')
		{
			isNegative = 1;
			i = 3;
		}
		result[0] = RAISE;
		while(i < strlen(msg))
		{
			result[1] *=10;
			result[1] +=msg[i] -'0';
			i++;
		}
		if(isNegative == 1)
			result[1] = 0 - result[1];
	}
	return result;
}
void firstplay(char *opponent,int sockfd)
{
	int newbet,highbet,rcvsize;
	int _continue = 1,count = 0;
	int *recive = (int*)calloc(4,sizeof(int));
	char *msg = (char*)calloc(MSG_SIZE,sizeof(char));
	int result;
	int newRound = 0;		
	while(_continue > 0)
	{
		if(count == 2)
		{
			Round++;
			newRound = 1;
			if(Round == 5)
			{
				printf("The bet phase is over\n");
				strcpy(msg,makeEndMessage(COMMON));
				send(sockfd,msg,strlen(msg),0);
				rcvsize = recv(sockfd,msg,MSG_SIZE,0);
				recive = processOpponentAction(msg);
				opponentcard[5] = newcard(recive[2]);
				opponentcard[6] = newcard(recive[3]);
				printf("GAME RESULT:\n");
				drawTable(4);
				printf("Your hand:\n");
				drawHand();
				printf("%s's hand:\n",opponent);
				drawOpponentHand();
				result = handCompare(calculate(card),calculate(opponentcard));
				if(result>0)
				{
					printf("You win this game. Your score is %d\n",chip + bet[1]);
				}
				else if(result<0)
				{
					printf("Player %s win this game\n",opponent);
				}
				else printf("Draw. Your score is %d\n",chip);
				getchar();
				printf("Press something to exit:....");
				gets(msg);
				return;
			}
			drawTable(Round);
			count = 0;
		}
		system("clear");
		drawTable(Round);
		drawHand();
		highbet = bet[0] >= bet[1] ? bet[0] : bet[1];
		memset(msg,0,sizeof(msg));
		printf("Your bet: %d-----%s's bet: %d\n",bet[0],opponent,bet[1] );
		printf("Your chip is: %d\n",chip);
		printf("Enter your new bet(enter -1 to fold):  ");
		scanf("%d",&newbet);
		if(newbet == -1)
		{
			printf("You have surrendered. The winner is %s\n",opponent );
			strcpy(msg,makeEndMessage(FOLD));
			send(sockfd,msg,strlen(msg),0);
			return;
		}
		while(newbet < highbet||newbet > chip)
		{
			if(newbet < highbet)
			{
				printf("Bet must be at least %d\n",highbet);
			}
			else
			{
				printf("You don't have enough chips to bet\n");
			}
			printf("Enter your new bet(enter -1 to fold):  ");
			scanf("%d",&newbet);			
		}
		bet[0] = newbet;
		if(newbet == chip)
		{
			printf("You're all in. The opponent is forced to all in or surrender.\n");			
			newbet = -2;
		}
		else if(newbet == 0 && highbet == 0)
			printf("You check\n");
		else if(newbet !=0 && highbet == 0)
			printf("You bet\n");
		else if(newbet == highbet)
			printf("You call\n");
		else if(newbet > highbet)
			printf("You raise\n");
		if(newRound ==0)
			strcpy(msg,makeBetMessage(newbet));
		else {
			strcpy(msg,makeNewRoundBetMessage(newbet));
			newRound = 0;
		}
		send(sockfd,msg,strlen(msg),0);
		printf("Waiting for opponent to reply\n");
		rcvsize = recv(sockfd,msg,MSG_SIZE,0);
		recive = processOpponentAction(msg);
		if(recive[0] == RAISE)
		{
			if(recive[1] == -1)
			{
				printf(" Player %s have surrendered. You win this game.\n",opponent );
				printf("Your score is %d\n",chip + bet[1]);
				getchar();
				printf("Press something to exit:....");
				gets(msg);
				return;
			}
			if(recive[1] == -2)
			{
				printf("Player %s has All-in. You must all-in or fold\n",opponent );
				printf("1. All-in\n2. Fold\n");
				scanf("%d",&newbet);
				while(newbet<1||newbet>2)
				{
					printf("Invalid selection. Re-enter:  ");
					scanf("%d",&newbet);
				}
				if(newbet == 1)
				{
					printf("You're All-in.\n");
					strcpy(msg,makeEndMessage(ALLIN));
					send(sockfd,msg,strlen(msg),0);
					rcvsize = recv(sockfd,msg,MSG_SIZE,0);
					recive = processOpponentAction(msg);
					opponentcard[5] = newcard(recive[2]);
					opponentcard[6] = newcard(recive[3]);
					printf("GAME RESULT:\n");
					drawTable(4);
					printf("Your hand:\n");
					drawHand();
					printf("%s's hand:\n",opponent);
					drawOpponentHand();
					result = handCompare(calculate(card),calculate(opponentcard));
					if(result>0)
					{
						printf("You win this game. Your score is %d\n",2*chip);
					}
					else if(result<0)
					{
						printf("Player %s win this game\n",opponent);
					}
					else printf("Draw. Your score is %d\n",chip);
					getchar();
					printf("Press something to exit:....");
					gets(msg);
					return;
				}
				else
				{
					printf("You have surrendered. Player %s win this game\n",opponent );
					strcpy(msg,makeEndMessage(FOLD));
					send(sockfd,msg,strlen(msg),0);
					getchar();
					printf("Press something to exit:....");
					gets(msg);
					return;
				}
			}
			else
			{
				bet[1] = recive[1];
			}
		}
		else
		{
			if(recive[1] == -1)
			{
				printf("Player %s have surrendered. You win this game. \n",opponent);
				printf("Your score is %d\n",chip + bet[1]);
				getchar();
				printf("Press something to exit:....");
				gets(msg);
				return;
			}
			else
			{
				opponentcard[5] = newcard(recive[2]);
				opponentcard[6] = newcard(recive[3]);
				printf("GAME RESULT:\n");
				drawTable(4);
				printf("Your hand:\n");
				drawHand();
				printf("%s's hand:\n",opponent);
				drawOpponentHand();
				result = handCompare(calculate(card),calculate(opponentcard));
				if(result>0)
				{
					printf("You win this game. Your score is %d\n",chip + bet[1]);
				}
				else if(result<0)
				{
					printf("Player %s win this game\n",opponent);
				}
				else printf("Draw. Your score is %d\n",chip);
				getchar();
				printf("Press something to exit:....");
				gets(msg);
				return;
			}
		}
		system("clear");
		if(bet[0] == bet[1])
			count++;
		else count = 0;
	}	
}

void secondplay(char *opponent,int sockfd)
{
	int newbet,highbet,rcvsize;
	int _continue = 1;
	int *recive = (int*)calloc(4,sizeof(int));
	char *msg = (char*)calloc(MSG_SIZE,sizeof(char));
	int result;
	system("clear");		
	while(_continue > 0)
	{
		printf("Waiting for opponent to reply\n");
		rcvsize = recv(sockfd,msg,MSG_SIZE,0);
		recive = processOpponentAction(msg);
		system("clear");
		drawTable(Round);
		drawHand();
		if(recive[0] == RAISE)
		{
			if(recive[1] == -1)
			{
				printf(" Player %s have surrendered. You win this game.\n",opponent );
				printf("Your score is %d\n",chip + bet[1]);
				printf("Press something to exit:....");
				gets(msg);
				return;
			}
			if(recive[1] == -2)
			{
				printf("Player %s has All-in. You must all-in or fold\n",opponent );
				printf("1. All-in\n2. Fold\n");
				scanf("%d",&newbet);
				while(newbet<1||newbet>2)
				{
					printf("Invalid selection. Re-enter:  ");
					scanf("%d",&newbet);
				}
				if(newbet == 1)
				{
					printf("You're All-in.\n");
					strcpy(msg,makeEndMessage(ALLIN));
					send(sockfd,msg,strlen(msg),0);
					rcvsize = recv(sockfd,msg,MSG_SIZE,0);
					recive = processOpponentAction(msg);
					opponentcard[5] = newcard(recive[2]);
					opponentcard[6] = newcard(recive[3]);
					printf("GAME RESULT:\n");
					drawTable(4);
					printf("Your hand:\n");
					drawHand();
					printf("%s's hand:\n",opponent);
					drawOpponentHand();
					result = handCompare(calculate(card),calculate(opponentcard));
					if(result>0)
					{
						printf("You win this game. Your score is %d\n",chip + bet[1]);
					}
					else if(result<0)
					{
						printf("Player %s win this game\n",opponent);
					}
					else printf("Draw. Your score is %d\n",chip);
					getchar();
					printf("Press something to exit:....");
					gets(msg);
					return;
				}
				else
				{
					printf("You have surrendered. Player %s win this game\n",opponent );
					strcpy(msg,makeEndMessage(FOLD));
					send(sockfd,msg,strlen(msg),0);
					printf("Press something to exit:....");
					getchar();
					gets(msg);
					return;
				}
			}
			else
			{
				bet[1] = recive[1];
			}
		}
		else
		{
			if(recive[1] == -1)
			{
				printf("Player %s have surrendered. You win this game. \n",opponent);
				printf("Your score is %d\n",chip + bet[1]);
				printf("Press something to exit:....");
				getchar();
				gets(msg);
				return;
			}
			else
			{
				opponentcard[5] = newcard(recive[2]);
				opponentcard[6] = newcard(recive[3]);
				printf("GAME RESULT:\n");
				drawTable(4);
				printf("Your hand:\n");
				drawHand();
				printf("%s's hand:\n",opponent);
				drawOpponentHand();
				result = handCompare(calculate(card),calculate(opponentcard));
				if(result>0)
				{
					printf("You win this game. Your score is %d\n",chip + bet[1]);
				}
				else if(result<0)
				{
					printf("Player %s win this game\n",opponent);
				}
				else printf("Draw. Your score is %d\n",chip);
				getchar();
				printf("Press something to exit:....");
				gets(msg);
				return;
			}
		}
		highbet = bet[0] >= bet[1] ? bet[0] : bet[1];
		memset(msg,0,sizeof(msg));
		printf("Your bet: %d-----%s's bet: %d\n",bet[0],opponent,bet[1]);
		printf("Your chip is: %d\n",chip);
		printf("Enter your new bet(enter -1 to fold):  ");
		scanf("%d",&newbet);
		if(newbet == -1)
		{
			printf("You have surrendered. The winner is %s\n",opponent );
			strcpy(msg,makeEndMessage(FOLD));
			send(sockfd,msg,strlen(msg),0);
			printf("Press something to exit:....");
			getchar();
			gets(msg);
			return;
		}
		while(newbet < highbet||newbet > chip)
		{
			if(newbet < highbet)
			{
				printf("Bet must be at least %d\n",highbet);
			}
			else
			{
				printf("You don't have enough chips to bet\n");
			}
			printf("Enter your new bet(enter -1 to fold):  ");
			scanf("%d",&newbet);			
		}
		bet[0] = newbet;
		if(newbet == chip)
		{
			printf("You're all in. The opponent is forced to all in or surrender.\n");			
			newbet = -2;
		}
		else if(newbet == 0 && highbet == 0)
			printf("You check\n");
		else if(newbet !=0 && highbet == 0)
			printf("You bet\n");
		else if(newbet == highbet)
			printf("You call\n");
		else if(newbet > highbet)
			printf("You raise\n");
		strcpy(msg,makeBetMessage(newbet));
		send(sockfd,msg,strlen(msg),0);
	}
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
	system("clear");
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
				configOpponent(msg,opponent);
				setPlay();
				setHandCard(msg);
				firstplay(opponent,sockfd);				
				system("clear");
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
					printf("Successfully join.\n");
					configOpponent2(msg,opponent);
					setPlay();
					setHandCard(msg);
					secondplay(opponent,sockfd);
				}
				if(msg[2]=='0' + FULL_SLOT)
					printf("Room is full\n");
				if(msg[2]=='0' + WRONG_RPASS)
					printf("Wrong password\n");
				if(msg[2]=='0' + ROOM_NEXIST)
					printf("Room does not exist\n");
				system("clear");
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