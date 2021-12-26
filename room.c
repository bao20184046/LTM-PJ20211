#include "room.h"
#include "handcard.h"
#include "card.h"
Room makeListRoom()
{
	Room new = (Room)calloc(1,sizeof(struct box));
	return new;
}
Player newPlayer(char* nickname,int money)
{
	Player new;
	strcpy(new.nickname,nickname);
	new.money = money;
	return new;
}

void joinRoom(Room headRoom, Player newPlayer)
{
	if(headRoom->canPlay==1)
	{
		printf("Full slot\n");
		return;
	}
	headRoom->player[1] = newPlayer;
	headRoom->canPlay = 1;
}

int checkRoom(Room headRoom)
{
	if(headRoom->player[0].money==0)
	{
		printf("Player %s failed\n", headRoom->player[0].nickname);
		return 2;
	}
	if(headRoom->player[1].money==0)
	{
		printf("Player %s failed\n", headRoom->player[1].nickname);
		return 1;
	}
	return 0;
}
void showPlayer(Room headRoom)
{
	printf("Player %s: %dVND\n",headRoom->player[0].nickname,headRoom->player[0].money);
	printf("Player %s: %dVND\n",headRoom->player[1].nickname,headRoom->player[1].money);
}

Room getRoombyID(Room headRoom, int id)
{
	Room r = headRoom;
	while(r!=NULL)
	{
		if(r->id == id)
			return r;
		r = r->next;
	}
	return NULL;
}

void pushRoom(Room headRoom, int status, int id, char* password, Player creator_room)
{
	Room new = (Room)calloc(1,sizeof(struct box));
	new->id = id;
	new->status = status;
	new->canPlay = 0;
	strcpy(new->password,password);
	new->player[0] = creator_room;
	new->next = NULL;
	if(headRoom == NULL)
	{
		headRoom = new;
	}
	else
	{
		Room r = headRoom;
		while(r->next!=NULL)
		{
			r=r->next;
		}
		r->next = new;
	}
}
void drawTable(int round, Card deck[],int start)
{
	if(round==0)
	{
		printf("------------------------------------------\n");
		printf("|\t| | \t| | \t| | \t| | \t|\n");
		printf("|\t| | \t| | \t| | \t| | \t|\n");
		printf("------------------------------------------\n");
	}
	if(round==1)
	{
		printf("------------------------------------------\n");
		printf("|%s\t| |%s\t| |%s\t| | \t| | \t|\n",deck[start].showvalue,deck[start+1].showvalue,deck[start+2].showvalue);
		printf("|%s\t| |%s\t| |%s\t| | \t| | \t|\n",deck[start].showtype,deck[start+1].showtype,deck[start+2].showtype);
		printf("------------------------------------------\n");
	}
	if(round==2)
	{
		printf("------------------------------------------\n");
		printf("|%s\t| |%s\t| |%s\t| |%s\t| | \t|\n",deck[start].showvalue,deck[start+1].showvalue,deck[start+2].showvalue,deck[start+3].showvalue);
		printf("|%s\t| |%s\t| |%s\t| |%s\t| | \t|\nn",deck[start].showtype,deck[start+1].showtype,deck[start+2].showtype,deck[start+3].showtype);
		printf("------------------------------------------\n");
	}
	if(round==3)
	{
		printf("------------------------------------------\n");
		printf("|%s\t| |%s\t| |%s\t| |%s\t| |%s\t|\n",deck[start].showvalue,deck[start+1].showvalue,deck[start+2].showvalue,deck[start+3].showvalue,deck[start+4].showvalue);
		printf("|%s\t| |%s\t| |%s\t| |%s\t| |%s\t|\n",deck[start].showtype,deck[start+1].showtype,deck[start+2].showtype,deck[start+3].showtype,deck[start+4].showtype);
		printf("------------------------------------------\n");
	}
}
void drawHand(Card hand[])
{
	printf("------------------\n");
	printf("|%s\t| |%s\t|\n",hand[0].showvalue,hand[1].showvalue);
	printf("|%s\t| |%s\t|\n",hand[0].showtype,hand[1].showtype);
	printf("------------------\n");
}
int* newround(Room headRoom,int bet[],Card playercard[][7])
{
	int* result = (int*)calloc(2,sizeof(int));
	result[0] = -1;//fold player
	result[1] = 1;//next round
	int allin = -1;//all-in player exist
	int count = 0;//check equal bet
	int slot = 0;//current player
	int raise;//new bet
	int high;//high bet
	while(1)
	{
		high = bet[0] >= bet[1] ? bet[0] : bet[1]; 
		printf("%s's turn:\n",headRoom->player[slot].nickname);
		drawHand(playercard[slot]);
		printf("%s's bet: %d --- %s's bet: %d\n",headRoom->player[0].nickname,bet[0],headRoom->player[1].nickname,bet[1] );
		printf("Enter your new raise(press -1 to fold):  ");
		scanf("%d",&raise);
		if(raise == headRoom->player[slot].money)
		{
			printf("You all-in\n");
			allin = slot;
			slot = slot == 1 ? 0 : 1;
			break;
		}
		if(raise == -1)
		{
			printf("You fold!!!\n");
			result[0] = slot;
			result[1] = 0;
			break;
		}
		while(raise < high)
		{
			printf("You cannot bet with this value(< high bet).re-raise: ");
			scanf("%d",&raise);
		}
		while(raise > headRoom->player[slot].money)
		{
			printf("You do not have enough money. Please re-raise: ");
			scanf("%d",&raise);
		}
		if(raise == bet[slot])
			printf("You check\n");
		else if(raise!=0 && high ==0)
			printf("You bet\n");
		else if(raise > high)
			printf("You raise\n");			
		else if(raise == high)
			printf("You call\n");
		bet[slot]=raise;
		if(bet[0] == bet[1])
			count++;
		else count = 0;
		if(count == 3)
			break;
		slot = slot == 1 ? 0 : 1;
	}
	if(allin != -1)
	{
		printf("%s's turn: \n", headRoom->player[slot].nickname);
		printf("Player %s had already all-in.\n", headRoom->player[allin].nickname);
		printf("You must choose fold or all-in: 1. All-in\t2. Fold\n");
		scanf("%d",&raise);
		result[1] = 0;
		if(raise==2)
		{
			printf("You fold!!!\n");
			result[0] = slot;
		}
		else
		{
			printf("You all-in!!!\n");
			if(headRoom->player[0].money > headRoom->player[1].money)
			{
				bet[0] = headRoom->player[1].money;
				bet[1] = bet[0];
				printf("Player %s recive %d VND\n",headRoom->player[0].nickname,headRoom->player[0].money-headRoom->player[1].money);
			}
			else if(headRoom->player[0].money < headRoom->player[1].money)
			{
				bet[0] = headRoom->player[0].money;
				bet[1] = bet[0];
				printf("Player %s recive %d VND\n",headRoom->player[1].nickname,headRoom->player[1].money-headRoom->player[0].money);
			}
			else 
			{
				bet[0] = headRoom->player[0].money;
				bet[1] = bet[0];
			}
		}
	}
	return result;
}
void showHand(Player player)
{
	printf("Name: %s\n",player.nickname);
	printf("Rank: %d|High: %d|Second: %d|Third: %d|Fouth: %d|Last: %d\n", player.hand.rank,player.hand.high_value,player.hand.second_value,player.hand.third_value,player.hand.fouth_value,player.hand.last_value);
}
void match(Room headRoom)
{
	int *deck = rollcard();
	int totalbet = 0;
	int *Round = (int*)calloc(2,sizeof(int));
	int bet[2] = {0,0};
	int fold = -1;
	Card Deck[DECK_SIZE];
	Card playercard[2][7];
	for(int i = 0; i < DECK_SIZE;i++)
	{
		Deck[i] = newcard(deck[i]);
	}
	for(int i = 0; i < 2;i++)
	{
		playercard[i][0] = Deck[i];
		playercard[i][1] = Deck[2 + i];
		for(int j = 2; j < 7; j++)
		{
			playercard[i][j] = Deck[2+j];
		}
	}
	printf("Round 1:\n");
	drawTable(0,Deck,4);
	Round = newround(headRoom,bet,playercard);
	if(Round[1] == 1){
		printf("Round 2:\n");
		drawTable(1,Deck,4);
		Round = newround(headRoom,bet,playercard);
	}
	if(Round[1] == 1){
		printf("Round 3:\n");
		drawTable(2,Deck,4);
		Round = newround(headRoom,bet,playercard);
	} 
	if(Round[1] == 1){
		printf("Round 4:\n");
		drawTable(3,Deck,4);
		Round = newround(headRoom,bet,playercard);
	}
	totalbet = bet[0] + bet[1];
	headRoom->player[0].money-=bet[0];
	headRoom->player[1].money-=bet[1];
	if(Round[0]!=-1)
	{
		if(Round[0] == 0)
		{
			printf("Player %s wins this match\n",headRoom->player[1].nickname );
			headRoom->player[1].money+=totalbet;
		}
		else
		{
			printf("Player %s wins this match\n",headRoom->player[0].nickname );
			headRoom->player[0].money+=totalbet;
		}
	}
	else
	{
		headRoom->player[0].hand = calculate(playercard[0]);
		headRoom->player[1].hand = calculate(playercard[1]);
		showHand(headRoom->player[0]);
		showHand(headRoom->player[1]);
		if(handCompare(headRoom->player[0].hand,headRoom->player[1].hand)>0)
		{
			printf("%s wins this match\n",headRoom->player[0].nickname );
			headRoom->player[0].money+=totalbet;
		}
		else if(handCompare(headRoom->player[0].hand,headRoom->player[1].hand)<0)
		{
			printf("%s wins this match\n",headRoom->player[1].nickname );
			headRoom->player[1].money+=totalbet;
		}
		else{
			printf("Draw:)!!!\n" );
			headRoom->player[1].money+=totalbet/2;
			headRoom->player[0].money+=totalbet/2;
		}
	}	
}