#ifndef __HANDCARD__
#define __HANDCARD__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "card.h"

typedef struct 
{
	int rank;//1-10 la tu sanh rong den mau thau
	int high_value;// so sanh cac tay bai co ranh bang nhau
	int second_value;//so sanh trong truong hop ca 2 
	int third_value;//so sanh con lai
	int fouth_value;
	int last_value;
}Handcard;

Handcard calculate(Card handcard[]);
int handCompare(Handcard hand1,Handcard hand2);

#endif