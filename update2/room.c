#include "room.h"
#include "handcard.h"
#include "card.h"
Room *makeListRoom()
{
	Room *new = (Room*)calloc(1,sizeof(struct box));
	return new;
}
Player newPlayer(char* nickname,int chip)
{
	Player new;
	strcpy(new.nickname,nickname);
	new.chip = chip;
	return new;
}

void joinRoom(Room *room, Player newPlayer)
{
	if(room->canPlay==1)
	{
		printf("Full slot\n");
		return;
	}
	room->player[1] = newPlayer;
	room->canPlay = 1;
}

Room *getRoombyID(Room *headRoom, int id)
{
	printf("%d\n", id);
	Room *r = headRoom;
	while(r!=NULL)
	{
		printf("%d)\n", r -> id);
		if(r->id == id)
			return r;
		r = r->next;
	}
	return NULL;
}
void setDeckToRoom(Room *room)
{
	memset(room->deck,0,sizeof(room->deck));
	int * deck = rollcard();
	for(int i = 0; i < 9; i++)
	{
		room->deck[i] = deck[i];
	}
}
int checkRoom(Room *room)
{
	if(room->player[0].chip==0)
	{
		printf("Player %s failed\n", room->player[0].nickname);
		return 2;
	}
	if(room->player[1].chip==0)
	{
		printf("Player %s failed\n", room->player[1].nickname);
		return 1;
	}
	return 0;
}
void showPlayer(Room *room)
{
	printf("Player %s: %dVND\n",room->player[0].nickname,room->player[0].chip);
	printf("Player %s: %dVND\n",room->player[1].nickname,room->player[1].chip);
}


Room *newRoom(int status, char* password, Player creator_room)
{
	Room *new = (Room*)malloc(sizeof(Room));
	memset(new,0,sizeof(new));
	new->status = status;
	new->canPlay = 0;
	strcpy(new->password,password);
	new->player[0] = creator_room;
	new->next = NULL;
	return new;
}
int pushRoom(Room **headRoom, int status, char* password, Player creator_room)
{
	int id = 0;
	Room *new = newRoom(status,password,creator_room);
	if(*headRoom == NULL)
	{
		*headRoom = new;
		(*headRoom)->id = 1;
		return 1;
	}
	Room *r = *headRoom;
	while(r->next!=NULL)
	{
		r = r->next;
	}
	r->next = new;
	id = r->id + 1;
	r->next->id = id;
	return id;
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
int* newround(Room *room,int bet[],Card playercard[][7])
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
		printf("%s's turn:\n",room->player[slot].nickname);
		drawHand(playercard[slot]);
		printf("%s's bet: %d --- %s's bet: %d\n",room->player[0].nickname,bet[0],room->player[1].nickname,bet[1] );
		printf("Enter your new raise(press -1 to fold):  ");
		scanf("%d",&raise);
		if(raise == room->player[slot].chip)
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
		while(raise > room->player[slot].chip)
		{
			printf("%d\n", room->player[slot].chip);
			printf("You do not have enough chip. Please re-raise: ");
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
		printf("%s's turn: \n", room->player[slot].nickname);
		printf("Player %s had already all-in.\n", room->player[allin].nickname);
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
			if(room->player[0].chip > room->player[1].chip)
			{
				bet[0] = room->player[1].chip;
				bet[1] = bet[0];
				printf("Player %s recive %d Chip\n",room->player[0].nickname,room->player[0].chip - room->player[1].chip);
			}
			else if(room->player[0].chip < room->player[1].chip)
			{
				bet[0] = room->player[0].chip;
				bet[1] = bet[0];
				printf("Player %s recive %d Chip\n",room->player[1].nickname,room->player[1].chip - room->player[0].chip);
			}
			else 
			{
				bet[0] = room->player[0].chip;
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
// void match(Room *room)
// {
// 	int *deck = rollcard();
// 	int totalbet = 0;
// 	int *Round = (int*)calloc(2,sizeof(int));
// 	room->bet[0] = 0;
// 	room->bet[1] = 0;
// 	int fold = -1;
// 	Card Deck[DECK_SIZE];
// 	Card playercard[2][7];
// 	for(int i = 0; i < 9;i++)
// 	{
// 		room->deck[i] = newcard(deck[i]);
// 	}
// 	for(int i = 0; i < 2;i++)
// 	{
// 		playercard[i][0] = room->deck[i];
// 		playercard[i][1] = room->deck[2 + i];
// 		for(int j = 2; j < 7; j++)
// 		{
// 			playercard[i][j] = room->deck[2+j];
// 		}
// 	}
// 	printf("Round 1:\n");
// 	drawTable(0,room->deck,4);
// 	Round = newround(room,room->bet,playercard);
// 	if(Round[1] == 1){
// 		printf("Round 2:\n");
// 		drawTable(1,room->deck,4);
// 		Round = newround(room,room->bet,playercard);
// 	}
// 	if(Round[1] == 1){
// 		printf("Round 3:\n");
// 		drawTable(2,room->deck,4);
// 		Round = newround(room,room->bet,playercard);
// 	} 
// 	if(Round[1] == 1){
// 		printf("Round 4:\n");
// 		drawTable(3,room->deck,4);
// 		Round = newround(room,room->bet,playercard);
// 	}
// 	totalbet = room->bet[0] + room->bet[1];
// 	room->player[0].chip-=room->bet[0];
// 	room->player[1].chip-=room->bet[1];
// 	if(Round[0]!=-1)
// 	{
// 		if(Round[0] == 0)
// 		{
// 			printf("Player %s wins this match\n",room->player[1].nickname );
// 			room->player[1].chip+=totalbet;
// 		}
// 		else
// 		{
// 			printf("Player %s wins this match\n",room->player[0].nickname );
// 			room->player[0].chip+=totalbet;
// 		}
// 	}
// 	else
// 	{
// 		room->player[0].hand = calculate(playercard[0]);
// 		room->player[1].hand = calculate(playercard[1]);
// 		showHand(room->player[0]);
// 		showHand(room->player[1]);
// 		if(handCompare(room->player[0].hand,room->player[1].hand)>0)
// 		{
// 			printf("%s wins this match\n",room->player[0].nickname );
// 			room->player[0].chip+=totalbet;
// 		}
// 		else if(handCompare(room->player[0].hand,room->player[1].hand)<0)
// 		{
// 			printf("%s wins this match\n",room->player[1].nickname );
// 			room->player[1].chip+=totalbet;
// 		}
// 		else{
// 			printf("Draw:)!!!\n" );
// 			room->player[1].chip+=totalbet/2;
// 			room->player[0].chip+=totalbet/2;
// 		}
// 	}	
// }