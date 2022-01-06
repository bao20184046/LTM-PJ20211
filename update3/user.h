#ifndef __USER__
#define __USER__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node{
	char username[30];
	char password[20];
	char nickname[20];
	struct node *next;
}User;

User *makeList();
User *getUserByUserName(User *head, char* username);
User *newUser(char* username, char* password, char* nickname);
void pushUser(User **head,char* username,char *password, char *nickname);
int numberNode(User *head);
#endif