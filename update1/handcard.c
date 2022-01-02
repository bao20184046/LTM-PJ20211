#include "handcard.h"



int* checkF(Card handcard[])//check thung
{
	int* result = (int*)calloc(6,sizeof(int));
	int count[] = {0,0,0,0};
	int filter[4][7];
	int max = 0,index1,index2 = 1;
	for(int i = 0; i < 7;i++)
	{
		if(handcard[i].type == 1)
		{
			filter[0][count[0]] = handcard[i].value;
			count[0]++;
		}
		else if(handcard[i].type == 2)
		{
			filter[1][count[1]] = handcard[i].value;
			count[1]++;
		}
		else if(handcard[i].type == 3)
		{
			filter[2][count[2]] = handcard[i].value;
			count[2]++;
		}
		else
		{
			filter[3][count[3]] = handcard[i].value;
			count[3]++;
		}
	}
	for(int i = 0; i < 4; i++)
	{
		if(max < count[i])
		{
			max = count[i];
			result[0] = i + 1;
		}
	}
	if(max<5)
	{
		memset(result,0,6*sizeof(int));
	}
	else
	{
		index1 = max - 1;
		if(filter[result[0]-1][0] == 1)
		{
			result[1] = 14;
			index2 = 2;
		}
		while(index2 < 6)
		{
			result[index2] = filter[result[0]-1][index1];
			index2++;
			index1--;
		}
	}
	return result;
}

int checkSF(Card handcard[])
{
	int* flush = checkF(handcard);
	int type = flush[0];
	int i = 0,count = 0,index = 0;
	if(type == 0)
		return 0;
	int straight[7];
	for(int i = 0; i < 7; i ++)
	{
		if(handcard[i].type == type)
		{
			straight[index] = handcard[i].value;
			index++;
		}
	}
	if(straight[0] == 1 &&straight[index-1] == 13&&straight[index-2] == 12 &&straight[index-3] == 11 &&straight[index-4] == 10)
	{
		return 14;
	}
	for(int i = 0; i < index - 1;i++)
	{
		if(straight[i] +1 == straight[i+1])
		{
			count++;
		}
		else
		{
			if(count>=4)
			{
				return straight[i];
			}
			count = 0;
		}
	}
	if(count>=4)
		return straight[index-1];
	return 0;	
}

int* checkFour(Card handcard[])
{
	int* result = (int*)calloc(2,sizeof(int));
	result[0] = 0;
	int count = 0,i = 1;
	while(i < 7)
	{
		if(handcard[i].value == handcard[i-1].value)
		{
			count++;
		}
		else
		{
			count = 0;
		}
		if(count == 3)
		{
			if(handcard[i-1].value == 1)
				result[0] =  14;
			result[0] = handcard[i-1].value;
		}
		i++;
	}
	i = 6;
	while(i>0)
	{
		if(handcard[i].value != result[0])
		{
			result[1] = handcard[i].value;
			break;
		}
		i--;
	}
	if(handcard[0].value==1&&result[0]!=14)
		result[1] = 14;
	return result;
}

int* checkPair(Card handcard[])
{
	int* result = (int*)calloc(4,sizeof(int));
	result[0]  = 0;
	int count = 0,i = 1;
	while(i < 7)
	{
		if(handcard[i].value == handcard[i - 1].value)
		{
			count++;
		}
		else
		{
			if(count==1)
			{
				if(handcard[i-1].value==1)
				{
					result[0] = 14;
				}
				else if(handcard[i-1].value > result[0])
				{
					result[0] = handcard[i-1].value;
				}
			}
			count = 0;
		}
		i++;
	}
	if(count==1&&handcard[6].value > result[0])
	{
		result[0] = handcard[6].value;
	}
	count = 1;
	i = 6;
	if(handcard[0].value==1&&result[0]!=14)
	{
		result[count++] = 14;
	}
	while(i > -1)
	{
		if(handcard[i].value != result[0])
		{
			result[count++] = handcard[i].value;
		}
		if(count > 3)
			break;
		i--;
	}
	return result;
}

int* checkThree(Card handcard[])
{
	int* result = (int*)calloc(3,sizeof(int));
	result[0] = 0;
	int count = 0,i = 1;
	while(i < 7)
	{
		if(handcard[i].value == handcard[i - 1].value)
		{
			count++;
		}
		else
		{
			if(count==2)
			{
				if(handcard[i-1].value==1)
				{
					result[0] = 14;
				}
				else if(handcard[i-1].value > result[0])
				{
					result[0] = handcard[i-1].value;
				}
			}
			count = 0;
		}
		i++;
	}
	if(count==2&&handcard[6].value > result[0])
	{
		result[0] = handcard[6].value;
	}
	count = 1;
	if(handcard[0].value==1&&result[0]!=14)
	{
		result[count++] = 14;
	}
	i = 6;
	while(i > -1)
	{
		if(handcard[i].value != result[0])
		{
			result[count++] = handcard[i].value;
		}
		if(count > 2)
			break;
		i--;
	}
	return result;
}

int* checkFH(Card handcard[])
{
	int* result = (int*)calloc(2,sizeof(int));
	int* pair = checkPair(handcard);
	int* three = checkThree(handcard);
	result[0] = three[0];
	result[1] = pair[0];
	if(result[0] == 0 || result[1] ==0)
	{
		result[0] = 0;
		result[1] = 0;
	}
	return result;
}

int checkS(Card handcard[])
{
	int count = 1,num = 0;
	int value[7];
	value[0] = handcard[0].value;
	for(int i = 1; i < 7; i++)
	{
		if(handcard[i].value != handcard[i-1].value)
		{
			value[count] = handcard[i].value;
			count++;
		}
	}
	if(value[0]==1&&value[count-1]==13&&value[count-2]==12&&value[count-3]==11&&value[count-4]==10)
		return 14;
	for(int i = 0; i < count - 1 ; i++ )
	{
		if(value[i]+ 1 == value[i+1])
			num++;
		else
		{
			if(num>=4)
				return value[i];
			num = 0;
		}
	}
	if(num>=4)
		return value[count - 1];
	return 0;
}

int* check2Pair(Card handcard[])
{
	int* result = (int*)calloc(3,sizeof(int));
	result[1] = 0;
	result[0] = 15;
	int count = 0;
	if(handcard[0].value==1&&handcard[1].value==1)
	{
		result[count++] = 14;
	}
	for(int i = 6; i > 0;i--)
	{
		if(handcard[i].value == handcard[i-1].value && handcard[i].value !=result[0])
		{
			result[count] = handcard[i].value;
			count++;
		}
		if(count == 2)
			break;
	}
	if(result[1] == 0||result[0] == 15)
	{
		result[0] = 0;
	}
	if(handcard[0].value==1)
	{
		result[2] = 14;
		return result;
	}
	count = 6;
	while(count>0)
	{
		if(handcard[count].value != result[0]&&handcard[count].value!=result[1])
		{
			result[2] = handcard[count].value;
			break;
		}
		count--;
	}
	return result;
}
int* checkHC(Card handcard[])
{
	int* result = (int*)calloc(5,sizeof(int));
	int count = 0;
	if(handcard[0].value == 1)
	{
		result[0] = 14;
		count++;
	}
	result[count] = handcard[6].value;
	count++;
	result[count] = handcard[5].value;
	count++;
	result[count] = handcard[4].value;
	count++;
	result[count] = handcard[3].value;
	count++;
	if(count!=5)
		result[count] = handcard[2].value;
	return result;
}
void sortHandCard(Card handcard[])
{
	Card temp;
	for(int i = 0; i < 6; i++)
	{
		for(int j = i+1; j < 7; j++)
		{
			if(handcard[i].value > handcard[j].value)
			{
				temp = handcard[i];
				handcard[i] = handcard[j];
				handcard[j] = temp;
			}
		}
	}
}

Handcard calculate(Card handcard[])
{
	Handcard new;
	memset(&new,0,sizeof(new));
	sortHandCard(handcard);
	int SF = checkSF(handcard),S = checkS(handcard);
	int* Four = checkFour(handcard);
	int* F = checkF(handcard);
	int* FH = checkFH(handcard);
	int* twoP = check2Pair(handcard);
	int* Three = checkThree(handcard);
	int* Pair = checkPair(handcard);
	int* HC = checkHC(handcard);
	if(SF ==14)//sanh rong
	{
		new.rank = 10;
		return new;
	}
	if(SF != 0)//thung pha sanh
	{
		new.rank = 9;
		new.high_value = SF;
		return new;
	}
	if(Four[0]!=0)//tu quy
	{
		new.rank = 8;
		new.high_value = Four[0];
		new.second_value = Four[1];
		return new;
	}
	if(FH[0]!=0)//full house
	{
		new.rank = 7;
		new.high_value = FH[0];
		new.second_value = FH[1];
		return new;
	}
	if(F[0]!=0)//sanh
	{
		new.rank = 6;
		new.high_value = F[1];
		new.second_value = F[2];
		new.third_value = F[3];
		new.fouth_value = F[4];
		new.last_value = F[5];
		return new;
	}
	if(S!=0)//thung
	{
		new.rank = 5;
		new.high_value = S;
		return new;
	}
	if(Three[0]!=0)//bo ba
	{
		new.rank = 4;
		new.high_value = Three[0];
		new.second_value = Three[1];
		new.third_value = Three[2];
		return new;
	}
	if(twoP[0]!=0)//2 doi
	{
		new.rank = 3;
		new.high_value = twoP[0];
		new.second_value = twoP[1];
		new.third_value = twoP[2];
		return new;
	}
	if(Pair[0]!=0)//doi
	{
		new.rank = 2;
		new.high_value = Pair[0];
		new.second_value = Pair[1];
		new.third_value = Pair[2];
		new.fouth_value = Pair[3];
		return new;
	}
	new.rank = 1;//mau thau
	new.high_value = HC[0];
	new.second_value = HC[1];
	new.third_value = HC[2];
	new.fouth_value = HC[3];
	new.last_value = HC[4];
	return new;
}
int compare(int num1,int num2)
{
	if(num1>num2)
		return 1;
	else if(num1<num2)
		return -1;
	return 0;
}
int handCompare(Handcard hand1,Handcard hand2)
{
	int result = compare(hand1.rank,hand2.rank);
	if(result!=0)
		return result;
	result = compare(hand1.high_value,hand2.high_value);
	if(result!=0)
		return result;
	result = compare(hand1.second_value,hand2.second_value);
	if(result!=0)
		return result;
	result = compare(hand1.third_value,hand2.third_value);
	if(result!=0)
		return result;
	result = compare(hand1.fouth_value,hand2.fouth_value);
	if(result!=0)
		return result;
	result = compare(hand1.last_value,hand2.last_value);
	if(result!=0)
		return result;
	return 0;
}
