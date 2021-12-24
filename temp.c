#include <stdio.h>
int change(int* a,int b[])
{

	*a= *a+6;
	b[1]= 8;
	b[0] = b[1];
	return 0;
}

int main(int argc, char const *argv[])
{
	int a = 3;
	int b[2] = {0,0};
	change(&a,b);
	printf("%d -%d -%d\n",a,b[0],b[1] );
	return 0;
}