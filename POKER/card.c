#include "card.h"



char* convertType(int type)
{
	if(type ==1)
		return "♠";
	if(type == 2)
		return "♣";
	if(type == 3)
		return "♦";
	if(type == 4)
		return "♥";
}

char* convertValue(int value)
{
	if(value== 1)
		return "A";
	if(value== 2)
		return "2";
	if(value== 3)
		return "3";
	if(value== 4)
		return "4";
	if(value== 5)
		return "5";
	if(value== 6)
		return "6";
	if(value== 7)
		return "7";
	if(value== 8)
		return "8";
	if(value== 9)
		return "9";
	if(value== 10)
		return "10";
	if(value== 11)
		return "J";
	if(value== 12)
		return "Q";
	if(value== 13)
		return "K";
	if(value== 14)
		return "A";
}

Card newcard(int input)
{
	Card new;	
	new.value = input%13+1;
	new.type =  input/13 + 1;
	strcpy(new.showvalue,convertValue(new.value));
	strcpy(new.showtype,convertType(new.type));
	return new;
}

int* rollcard()
{
	sleep(1);
	int* deck = (int*)calloc(DECK_SIZE,sizeof(int));
	int j, k, temp;
	for(int i = 0; i < 52; i++)
	{
		deck[i] = i;
	}
	srand(time(NULL));
	for(int i = 0;i<200; i++)
	{
		j = rand()%DECK_SIZE;
		k = rand()%DECK_SIZE;
		temp = deck[j];
		deck[j] = deck[k];
		deck[k] = temp;
	}
	return deck;
}