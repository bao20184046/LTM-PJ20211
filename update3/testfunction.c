#include <stdio.h>
#include <unistd.h>   

#define SIZE 100


int main(int argc, char const *argv[])
{
	char *pass = getpass("Password: ");
	printf("%s\n",pass );
	return 0;
}