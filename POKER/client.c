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
Card pre_card[7];
Card pre_opponentcard[7];
int bet[2];
int Round;
int chip;
Handcard hand1;
Handcard hand2;
void menu()
{
	printf("***************************************\n");
	printf("MENU:\n");
	printf("1. Create room\n");
	printf("2. Join room\n");
	printf("3. View score\n");
	printf("4. View last battle result\n");
	printf("5. Log out\n");
	printf("***************************************\n");
	printf("Enter your choice:  ");
}

void SignIU()
{
	printf("***************************************\n");
	printf("Welcome to the world of poker\n");
	printf("1. Sign in\n");
	printf("2. Sign up\n");
	printf("3. Exit\n");
	printf("***************************************\n");
	printf("Enter your choice: \n");
}

void setHandCard(char *msg)
{
	memset(card,0,sizeof(card));
	int i = 0,j=0;
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
	memset(pre_card,0,sizeof(card));
	memset(pre_opponentcard,0,sizeof(opponentcard));
	memset(&hand1,0,sizeof(hand1));
	memset(&hand2,0,sizeof(hand2));
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
		printf("|%s\t| |%s\t| |%s\t| | \t| | \t|\n",pre_card[0].showvalue,pre_card[1].showvalue,pre_card[2].showvalue);
		printf("|%s\t| |%s\t| |%s\t| | \t| | \t|\n",pre_card[0].showtype,pre_card[1].showtype,pre_card[2].showtype);
		printf("------------------------------------------\n");
	}
	if(round==3)
	{
		printf("------------------------------------------\n");
		printf("|%s\t| |%s\t| |%s\t| |%s\t| | \t|\n",pre_card[0].showvalue,pre_card[1].showvalue,pre_card[2].showvalue,pre_card[3].showvalue);
		printf("|%s\t| |%s\t| |%s\t| |%s\t| | \t|\nn",pre_card[0].showtype,pre_card[1].showtype,pre_card[2].showtype,pre_card[3].showtype);
		printf("------------------------------------------\n");
	}
	if(round==4)
	{
		printf("------------------------------------------\n");
		printf("|%s\t| |%s\t| |%s\t| |%s\t| |%s\t|\n",pre_card[0].showvalue,pre_card[1].showvalue,pre_card[2].showvalue,pre_card[3].showvalue,pre_card[4].showvalue);
		printf("|%s\t| |%s\t| |%s\t| |%s\t| |%s\t|\n",pre_card[0].showtype,pre_card[1].showtype,pre_card[2].showtype,pre_card[3].showtype,pre_card[4].showtype);
		printf("------------------------------------------\n");
	}
	printf("****************************************\n");
}
void drawHand()
{
	printf("------------------\n");
	printf("|%s\t| |%s\t|\n",pre_card[5].showvalue,pre_card[6].showvalue);
	printf("|%s\t| |%s\t|\n",pre_card[5].showtype,pre_card[6].showtype);
	printf("------------------\n");
}
void drawOpponentHand()
{
	printf("------------------\n");
	printf("|%s\t| |%s\t|\n",pre_opponentcard[5].showvalue,pre_opponentcard[6].showvalue);
	printf("|%s\t| |%s\t|\n",pre_opponentcard[5].showtype,pre_opponentcard[6].showtype);
	printf("------------------\n");
}
void analysis(Handcard hand, int option)
{
	char buff[10];
	if(option == 1)
		strcpy(buff,"You");
	else strcpy(buff,"Opponent");
	if(hand.rank == 10)
		printf("%s have Royal Flush(Rank 10). %s have Straight from 10 to A of the same suit.\n",buff,buff);
	if(hand.rank == 9)
		printf("%s have Straight Flush(Rank 9). %s have Straight from %s to %s of the same suit.\n",buff,buff,convertValue(hand.high_value-4),convertValue(hand.high_value));
	if(hand.rank == 8)
		printf("%s have Four of a kind(Rank 8). %s have 4 cards of %s. The remaining highest value card is %s\n",buff,buff,convertValue(hand.high_value),convertValue(hand.second_value));
	if(hand.rank == 7)
		printf("%s have Fullhouse(Rank 7). %s have 3 cards of %s and pair of %s.\n",buff,buff,convertValue(hand.high_value),convertValue(hand.second_value));
	if(hand.rank == 6)
		printf("%s have Flush(Rank 6). %s have 5 cards of the same suit as %s, %s, %s, %s, %s.\n",buff,buff,convertValue(hand.high_value),convertValue(hand.second_value),convertValue(hand.third_value),convertValue(hand.fouth_value),convertValue(hand.last_value));
	if(hand.rank == 5)
		printf("%s have Straight(Rank 5). %s have Straight from %s to %s.\n",buff,buff,convertValue(hand.high_value-4),convertValue(hand.high_value));
	if(hand.rank == 4)
		printf("%s have Three of a kind(Rank 4). %s have 3 cards of %s.The remaining highest value cards are %s and %s.\n",buff,buff,convertValue(hand.high_value),convertValue(hand.second_value),convertValue(hand.third_value));
	if(hand.rank == 3)
		printf("%s have Two pair(Rank 3). %s have pair of %s and %s. The remaining highest value card is %s.\n",buff,buff,convertValue(hand.high_value),convertValue(hand.second_value),convertValue(hand.third_value));
	if(hand.rank == 2)
		printf("%s have Pair(Rank 2). %s have pair of %s. The remaining highest value cards are %s, %s and %s.\n",buff,buff,convertValue(hand.high_value),convertValue(hand.second_value),convertValue(hand.third_value),convertValue(hand.fouth_value));
	if(hand.rank == 1)
		printf("%s have High Card(Rank 1). 5 most valuable cards are %s, %s, %s, %s, %s.\n",buff,convertValue(hand.high_value),convertValue(hand.second_value),convertValue(hand.third_value),convertValue(hand.fouth_value),convertValue(hand.last_value));
}
void copyHand()
{
	for(int i = 0; i < 7; i++)
	{
		pre_card[i] = card[i];
		pre_opponentcard[i] = opponentcard[i];
	}
}
void ViewLastBattleResult()
{
	if(pre_card[0].value==0)
	{
		printf("When you open this application, you have not fought any battles.\n");
		return;
	}
	printf("GAME RESULT:\n");
	drawTable(4);
	printf("Your hand:\n");
	drawHand();
	printf("Opponent's hand:\n");
	drawOpponentHand();
	printf("Information:\n");
	analysis(hand1,1);
	printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	analysis(hand2,2);
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
char* statusRoom(int status)
{
	if(status == 0)
		return "Public";
	return "Private";
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
			printf(" %d\t%s\n",id,statusRoom(status));
		else printf("%d\t%s\n",id,statusRoom(status));
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
void countdown(int option)
{
	char buff[30];
	if(option==1)
		strcpy(buff,"The game starts");
	else strcpy(buff,"return to the main screen");
	printf("%s in 3 second\n",buff);
	sleep(1);
	printf("%s in 2 second\n",buff);
	sleep(1);
	printf("%s in 1 second\n",buff);
	sleep(1);
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
	printf("Player %s has entered your room!!!\n",opponent);
	countdown(1);
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
	printf("you have entered player %s's room!!!\n",opponent);
	countdown(1);
}
int getScore(char *msg)
{
	int i = 2;
	int score = 0;
	while(i < strlen(msg))
	{
		score*=10;
		score+=msg[i] - '0';
		i++;
	}
	return score;
}
int* processOpponentAction(char* msg)
{
	int i = 2;
	int isNegative = 0;
	int *result = (int*)calloc(4,sizeof(int));
	if(msg[0] == '0' + END_RES)
	{
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
int firstplay(char *opponent,int sockfd)
{
	int newbet,highbet;
	int count = 0;
	int *recive = (int*)calloc(4,sizeof(int));
	char *msg = (char*)calloc(MSG_SIZE,sizeof(char));
	int result;
	int newRound = 0;		
	copyHand();
	while(1)
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
				recv(sockfd,msg,MSG_SIZE,0);
				recive = processOpponentAction(msg);
				opponentcard[5] = newcard(recive[2]);
				opponentcard[6] = newcard(recive[3]);
				copyHand();
				system("clear");
				printf("GAME RESULT:\n");
				drawTable(4);
				printf("Your hand:\n");
				drawHand();
				printf("%s's hand:\n",opponent);
				drawOpponentHand();				
				hand1 = calculate(card);
				hand2 = calculate(opponentcard);
				result = handCompare(hand1,hand2);
				if(result>0)
				{
					printf("You win this game. Your score is %d\n",chip + bet[1]);
					return chip + bet[1];
				}
				else if(result<0)
				{
					printf("Player %s win this game\n",opponent);
					return chip - bet[0];
				}
				else printf("Draw. Your score is %d\n",chip);
				return chip;
			}
			drawTable(Round);
			count = 0;
		}
		system("clear");
		drawTable(Round);
		drawHand();
		highbet = bet[0] >= bet[1] ? bet[0] : bet[1];
		printf("Your bet: %d-----%s's bet: %d\n",bet[0],opponent,bet[1] );
		printf("Your chip is: %d\n",chip);
		printf("Enter your new bet(enter -1 to fold):  ");
		scanf("%d",&newbet);
		if(newbet == -1)
		{
			printf("You have surrendered. The winner is %s\n",opponent );
			strcpy(msg,makeEndMessage(FOLD));
			send(sockfd,msg,strlen(msg),0);
			return 0;
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
		recv(sockfd,msg,MSG_SIZE,0);
		recive = processOpponentAction(msg);
		if(recive[0] == RAISE)
		{
			if(recive[1] == -1)
			{
				printf(" Player %s have surrendered. You win this game.\n",opponent );
				printf("Your score is %d\n",chip + bet[1]);
				return chip + bet[1];
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
					recv(sockfd,msg,MSG_SIZE,0);
					recive = processOpponentAction(msg);
					opponentcard[5] = newcard(recive[2]);
					opponentcard[6] = newcard(recive[3]);
					system("clear");
					copyHand();
					printf("GAME RESULT:\n");
					drawTable(4);
					printf("Your hand:\n");
					drawHand();
					printf("%s's hand:\n",opponent);
					drawOpponentHand();					
					hand1 = calculate(card);
					hand2 = calculate(opponentcard);
					result = handCompare(hand1,hand2);
					if(result>0)
					{
						printf("You win this game. Your score is %d\n",2*chip);
						return 2*chip;
					}
					else if(result<0)
					{
						printf("Player %s win this game\n",opponent);
						return 0;
					}
					else printf("Draw. Your score is %d\n",chip);
					return chip;
				}
				else
				{
					printf("You have surrendered. Player %s win this game\n",opponent );
					strcpy(msg,makeEndMessage(FOLD));
					send(sockfd,msg,strlen(msg),0);
					return 0;
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
				return chip + bet[1];
			}
			else
			{
				opponentcard[5] = newcard(recive[2]);
				opponentcard[6] = newcard(recive[3]);
				printf("GAME RESULT:\n");
				copyHand();
				drawTable(4);
				printf("Your hand:\n");
				drawHand();
				printf("%s's hand:\n",opponent);
				drawOpponentHand();				
				hand1 = calculate(card);
				hand2 = calculate(opponentcard);
				result = handCompare(hand1,hand2);
				if(result>0)
				{
					if(newbet == -2)
					{
						printf("You win this game. Your score is %d\n",chip*2);
						return chip*2;
					}
					printf("You win this game. Your score is %d\n",chip + bet[1]);
					return chip + bet[1];
				}
				else if(result<0)
				{
					printf("Player %s win this game\n",opponent);
					if(newbet == -2)
					{
						return 0;
					}
					return chip - bet[0];
				}
				else printf("Draw. Your score is %d\n",chip);
				return chip;
			}
		}
		system("clear");
		if(bet[0] == bet[1])
			count++;
		else count = 0;
	}	
}

int secondplay(char *opponent,int sockfd)
{
	int newbet,highbet;
	int *recive = (int*)calloc(4,sizeof(int));
	char *msg = (char*)calloc(MSG_SIZE,sizeof(char));
	int result;
	system("clear");
	copyHand();		
	while(1)
	{
		printf("Waiting for opponent to reply\n");
		recv(sockfd,msg,MSG_SIZE,0);
		recive = processOpponentAction(msg);
		system("clear");
		drawTable(Round);
		drawHand();
		if(recive[0] == RAISE)
		{
			if(recive[1] == -1)
			{
				printf(" Player %s have surrendered. You win this game.\n",opponent );
				return chip + bet[1];
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
					recv(sockfd,msg,MSG_SIZE,0);
					recive = processOpponentAction(msg);
					opponentcard[5] = newcard(recive[2]);
					opponentcard[6] = newcard(recive[3]);
					system("clear");
					copyHand();
					printf("GAME RESULT:\n");
					drawTable(4);
					printf("Your hand:\n");
					drawHand();
					printf("%s's hand:\n",opponent);
					drawOpponentHand();					
					hand1 = calculate(card);
					hand2 = calculate(opponentcard);
					result = handCompare(hand1,hand2);
					if(result>0)
					{
						printf("You win this game. Your score is %d\n",chip*2);
						return chip*2;
					}
					else if(result<0)
					{
						printf("Player %s win this game\n",opponent);
						return 0;
					}
					else printf("Draw. Your score is %d\n",chip);
					return chip;
				}
				else
				{
					printf("You have surrendered. Player %s win this game\n",opponent );
					strcpy(msg,makeEndMessage(FOLD));
					send(sockfd,msg,strlen(msg),0);
					return 0;
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
				return chip + bet[1];
			}
			else
			{
				opponentcard[5] = newcard(recive[2]);
				opponentcard[6] = newcard(recive[3]);
				system("clear");
				copyHand();
				printf("GAME RESULT:\n");
				drawTable(4);
				printf("Your hand:\n");
				drawHand();
				printf("%s's hand:\n",opponent);
				drawOpponentHand();				
				hand1 = calculate(card);
				hand2 = calculate(opponentcard);
				result = handCompare(hand1,hand2);
				if(result>0)
				{
					if(newbet == -2)
					{
						printf("You win this game. Your score is %d\n",chip*2);
						return chip*2;
					}
					printf("You win this game. Your score is %d\n",chip + bet[1]);
					return chip + bet[1];
				}
				else if(result<0)
				{
					printf("Player %s win this game\n",opponent);
					if(newbet == -2)
					{
						return 0;
					}
					return chip - bet[0];
				}
				else printf("Draw. Your score is %d\n",chip);
				return chip;
			}
		}
		highbet = bet[0] >= bet[1] ? bet[0] : bet[1];
		printf("Your bet: %d-----%s's bet: %d\n",bet[0],opponent,bet[1]);
		printf("Your chip is: %d\n",chip);
		printf("Enter your new bet(enter -1 to fold):  ");
		scanf("%d",&newbet);
		if(newbet == -1)
		{
			printf("You have surrendered. The winner is %s\n",opponent );
			strcpy(msg,makeEndMessage(FOLD));
			send(sockfd,msg,strlen(msg),0);
			return 0;
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
	int plus;
	//Set Address Information
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(6666);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//Link to server
	int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	assert(res != -1);
	while(1)
	{
		while(isLoged_in == 0)
		{
			SignIU();
			scanf("%d",&choice);
			while(choice<1||choice>3)
			{
				printf("Invalid selection. Re-enter:  ");
				scanf("%d",&choice);
			}
			getchar();
			system("clear");
			switch(choice)
			{
				case 1:
				{
					strcpy(msg,makeSignInMessage());
					send(sockfd,msg,strlen(msg),0);
					rcvsize = recv(sockfd,msg,MSG_SIZE,0);
					msg[rcvsize] = '\0';
					system("clear");
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
					else if(msg[2] == '0' + LOGED_IN)
					{
						printf("This account is logged in somewhere else\n");
					}
					break;
				}
				case 2:
				{
					strcpy(msg,makeSignUpMessage());
					send(sockfd,msg,strlen(msg),0);
					rcvsize = recv(sockfd,msg,MSG_SIZE,0);
					msg[rcvsize] = '\0';
					system("clear");
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
				case 3:
				{
					return 0;
				}
			}
		}
		system("clear");
		setPlay();
		printf("Login successfully. Welcome %s\n",nickname );
		while(isLoged_in == 1)
		{
			menu();
			scanf("%d",&choice);
			while(choice<1||choice>5)
			{
				printf("Invalid selection. Re-enter:  ");
				scanf("%d",&choice);
			}
			system("clear");
			switch(choice)
			{
				case 1:
				{
					
					strcpy(msg,makeCreateRoomMessage(nickname));
					if(strcmp(msg,"")==0)
						break;
					send(sockfd,msg,strlen(msg),0);
					rcvsize = recv(sockfd,msg,MSG_SIZE,0);
					msg[rcvsize] = '\0';
					showIDCreatedRoom(msg);
					printf("Please wait for another player to enter the room.\n");
					rcvsize = recv(sockfd,msg,MSG_SIZE,0);
					system("clear");
					configOpponent(msg,opponent);
					setPlay();
					setHandCard(msg);
					plus = firstplay(opponent,sockfd);
					strcpy(msg,makePlusScoreMessage(nickname,plus));
					send(sockfd,msg,strlen(msg),0);
					countdown(2);
					system("clear");
					printf("You get %d points added to your account\n",plus);					
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
						system("clear");
						configOpponent2(msg,opponent);
						setPlay();
						setHandCard(msg);
						plus = secondplay(opponent,sockfd);
						countdown(2);
						system("clear");
						printf("You get %d points added to your account\n",plus);
						strcpy(msg,makePlusScoreMessage(nickname,plus));
						send(sockfd,msg,strlen(msg),0);
					}
					else
					{
						system("clear");
						if(msg[2]=='0' + FULL_SLOT)
							printf("Room is full\n");
						if(msg[2]=='0' + WRONG_RPASS)
							printf("Wrong password\n");
						if(msg[2]=='0' + ROOM_NEXIST)
							printf("Room does not exist\n");
						
					}				
					break;
				}
				case 3:
				{
					strcpy(msg,getScoreMessage(nickname));
					send(sockfd,msg,strlen(msg),0);
					rcvsize = recv(sockfd,msg,MSG_SIZE,0);
					plus = getScore(msg);
					printf("Your score is %d\n",plus);
					break;
				}
				case 4:
				{
					ViewLastBattleResult();
					break;
				}
				case 5:
				{
					system("clear");
					printf("Goodbye %s\n",nickname);
					strcpy(msg,makeLogOutMessage(nickname));
					send(sockfd,msg,strlen(msg),0);
					isLoged_in = 0;
					break;
				}
			}
		}
	}	
	close(sockfd);
}