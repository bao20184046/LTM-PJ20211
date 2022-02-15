#ifndef __CARD__
#define __CARD__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define DECK_SIZE 52

typedef struct {
	int value;
	int type;//1-bich,2-tep,3-ro
	char showvalue[3];
	char showtype[3];
}Card;

char* convertType(int type);

char* convertValue(int value);

Card newcard(int input);

int* rollcard();

#endif