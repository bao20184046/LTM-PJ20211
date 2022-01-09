#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "protocol.h"

int main(int argc, char const *argv[])
{
	printf("%s\n",makePlusScoreMessage("bao", 200));
	return 0;
}