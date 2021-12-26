#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"

User *headUser = NULL;

User *makeList()
{
	User *acc = (User *)calloc(1,sizeof(User));
	return acc;
}
User *getUserByUserName(User *head, char* username)
{
	User *acc = head;
	while(acc != NULL)
	{
		if(strcmp(acc->username,username) == 0)
			return acc;
		acc= acc->next;
	}
	return NULL;
}
User *newUser(char* username, char* password, char* nickname)
{
	User *acc = (User *)calloc(1,sizeof(User));
	strcpy(acc->username,username);
	strcpy(acc->password,password);
	strcpy(acc->nickname,nickname);
	acc->next = NULL;
	return acc;
}
void pushUser(User *head,char* username,char *password, char *nickname)
{
	if(getUserByUserName(head,username)!=NULL)
	{
		return;
	}
	User *new = newUser(username, password, nickname);
	User *acc = head;
	if(head == NULL)
	{
		head = new;
	}
	else
	{
		while(acc->next!=NULL)
			acc = acc->next;
		acc->next = new;
	}
}

