#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/time.h>
#include "protocol.h"
#include "user.h"
#include "room.h"
#include "handcard.h"
#include "card.h"
#define MAXFD 10	//Size of fds array
#define PORT 6666	
User* headUser = NULL;
Room *headRoom = NULL;
typedef struct{
	int i1;
	int i2;
}connectt;
connectt Link[20];
void LoadUser()
{
	FILE *file = fopen("user.txt","r");
	char username[30],password[20],nickname[20];
	int score;
	while(!feof(file))
	{
		fscanf(file,"%s %s %s %d\n",username,password,nickname,&score);
		pushUser(&headUser,username,password,nickname,score);
	}
	fclose(file);
}

void updateUser()
{
	FILE* file = fopen("user.txt","w");
	User *temp = headUser;
	while(temp->next!=NULL)
	{
		fprintf(file,"%s %s %s %d\n",temp->username,temp->password,temp->nickname,temp->score);
		temp=temp->next;
	}
	fprintf(file,"%s %s %s %d",temp->username,temp->password,temp->nickname,temp->score);
	fclose(file);
}

void fds_add(int fds[],int fd)
{
	int i=0;
	for(;i<MAXFD;++i)
	{
		if(fds[i]==-1)
		{
	      fds[i]=fd;
		  break;
		}
	}
}
//0username password nickname
int processSignUp(char *msg,char *nick)
{
	char *username = (char*)calloc(30,sizeof(char));
	char *password = (char*)calloc(20,sizeof(char));
	char *nickname = (char*)calloc(20,sizeof(char));
	int i = 1,j = 0;
	while(msg[i]!=' ')
	{
		username[j] = msg[i];
		j++;
		i++;
	}
	username[j] = '\0';
	if(getUserByUserName(headUser,username)!=NULL)
	{
		strcpy(nick,"\0");
		return 0;
	}
	j = 0;i++;
	while(msg[i]!=' ')
	{
		password[j] = msg[i];
		j++;
		i++;
	}
	password[j] = '\0';
	j = 0;i++;
	while(msg[i]!='\0')
	{
		nickname[j] = msg[i];
		j++;
		i++;
	}
	nickname[j] = '\0';
	strcpy(nick,nickname);
	pushUser(&headUser,username,password,nickname,0);
	User * user = getUserByUserName(headUser,username);
	user->isLogin = 1;
	return 1;
}
int getOpponentIndex(int socket)
{
	for(int i = 1; i < 20; i++)
	{
		if(socket == Link[i].i1)
			return Link[i].i2;
		if(socket == Link[i].i2)
			return Link[i].i1;
	}
	return 0;
}
int processSignIn(char *msg,char *nick)
{
	char *username = (char*)calloc(30,sizeof(char));
	char *password = (char*)calloc(20,sizeof(char));
	User *loginUser = NULL;
	int i = 1,j = 0;
	while(msg[i]!=' ')
	{
		username[j] = msg[i];
		j++;
		i++;
	}
	username[j] = '\0';
	loginUser = getUserByUserName(headUser,username);
	if(loginUser==NULL)
	{
		strcpy(nick,"\0");
		return 0;
	}
	j = 0;i++;
	while(msg[i]!='\0')
	{
		password[j] = msg[i];
		j++;
		i++;
	}
	password[j] = '\0';
	j = 0;i++;
	if(strcmp(loginUser->password,password)!=0)
	{
		strcpy(nick,"\0");
		return 1;
	}
	if(loginUser->isLogin == 1)
	{
		strcpy(nick,"\0");
		return -1;
	}
	strcpy(nick,loginUser->nickname);
	loginUser->isLogin = 1;
	return 2;
}
char *signIURes(RES_TYPE type,VALUE_RES res,char *nickname)
{
	char *msg = (char*)calloc(25,sizeof(char));
	int i = 4;
	msg[0] = '0' + type;
	msg[1] = ' ';
	msg[2] = '0' + res;
	msg[3] = ' ';
	while(i-4 < strlen(nickname))
	{
		msg[i] = nickname[i-4];
		i++;
	}
	msg[i] = '\0';
	return msg;
}
char *listRoomRes()
{
	int temp,i = 0;
	char *str = (char*)calloc(20,sizeof(char));
	Room *r = headRoom;
	while(r!=NULL)
	{
		if(r->canPlay==0)
		{
			temp = r->id;
			while(temp!=0)
			{
				str[i++] = '0' + temp%10;
				temp/=10;
			}
			str[i++] = '-';
			str[i++] = '0' + r->status;
			str[i++] = '|';
		}
		r = r->next;
	}
	str[i-1] = '\0';
	return str;
}
char *createRoomRes(int ruler)
{
	int i = 2,id = ruler;
	char *msg = (char*)calloc(6,sizeof(char));
	msg[0] = '0' + CRE_RES;
	msg[1] = ' ';
	while(id!=0)
	{
		msg[i++] = '0'+id%10;
		id/=10;
	}
	msg[i] = '\0';
	return msg;
}
int getIdbyIndex(int socket)
{
	for(int i = 1; i < 20; i++)
	{
		if(socket == Link[i].i1||socket == Link[i].i2)
			return i;
	}
	return 0;
}
int processEnd(char *msg,int id)
{
	int type = msg[2] - '0';
	if(type == FOLD)
		return 0;
	if(type == OUT)
		return -1;
	return 1;
}
void processLogOut(char *msg)
{
	int i =2;
	char *nickname = (char*)calloc(20,sizeof(char));
	while(i < strlen(msg))
	{
		nickname[i-2] = msg[i];
		i++;
	}
	nickname[i] = '\0';
	User *user = getUserByNickName(headUser,nickname);
	user->isLogin = 0;
}
int processCreateRoom(char *msg)
{
	int j,i = 4,status = msg[2]-'0';
	int id;
	char *nickname = (char*)calloc(20,sizeof(char));
	char *password = (char*)calloc(20,sizeof(char));
	while(msg[i]!=' '&&i < strlen(msg))
	{
		nickname[i-4] = msg[i];
		i++;
	}
	nickname[i-4] = '\0';
	if(status == 1)
	{
		i++;
		j = i;
		while(i < strlen(msg))
		{
			password[i-j] = msg[i];
			i++;
		}
		password[i-j] = '\0';
		id = pushRoom(&headRoom,1,password,nickname);
		return id;
	}
	strcpy(password,"\0");
	id = pushRoom(&headRoom,0,password,nickname);
	return id;
}
void processPlusScore(char *msg)
{
	int i = 2,plus =0;
	char *nickname = (char*)calloc(20,sizeof(char));
	while(msg[i]!=' ')
	{
		nickname[i-2] = msg[i];
		i++;
	}
	nickname[i-2] = '\0';
	User *user = getUserByNickName(headUser,nickname);
	i++;
	while(i < strlen(msg))
	{
		plus*=10;
		plus+= msg[i] - '0';
		i++;
	}
	user->score+=plus;
}
int processGetScore(char *msg)
{
	int i = 2;
	char *nickname = (char*)calloc(20,sizeof(char));
	while(i < strlen(msg))
	{
		nickname[i-2] = msg[i];
		i++;
	}
	nickname[i-2] = '\0';
	User *user = getUserByNickName(headUser,nickname);
	return user->score;
}
char *scoreMessage(int score)
{
	int i = 2;
	char *str = (char*)calloc(10,sizeof(char));
	char *temp = (char*)calloc(4,sizeof(char));
	sprintf(temp,"%d",score);
	str[0] = '0' + SCORE_RES;
	str[1] = ' ';
	while(i-2 <strlen(temp))
	{
		str[i] = temp[i-2];
		i++;
	}
	str[i] = '\0';
	return str;
}

int processJoinRoom(char *msg)
{
	int id=0,i = 2,j;
	char *nickname = (char*)calloc(20,sizeof(char));
	char *password = (char*)calloc(20,sizeof(char));
	while(msg[i]!=' ')
	{
		nickname[i-2] = msg[i];
		i++;
	}
	nickname[i] = '\0';
	i++;
	while(msg[i]!=' '&&i<strlen(msg))
	{
		id*=10;
		id+=msg[i]-'0';
		i++;
	}
	Room *r = getRoombyID(headRoom,id);
	if(r==NULL)
	{
		return 0;
	}
	if(r->canPlay==1)
	{
		return -1;
	}
	if(r->status==1)
	{
		i++;
		j = i;
		while(i < strlen(msg))
		{
			password[i-j] = msg[i];
			i++;
		}
		password[i-j] = '\0';
		if(strcmp(r->password,password)==0)
		{
			joinRoom(r,nickname);
			setDeckToRoom(r);
			return id;
		}
		else
		{
			return -2;
		}
	}
	joinRoom(r,nickname);
	setDeckToRoom(r);
	return id;
}
char *noticeRes(int id)
{
	int i = 2,p;
	char *str = (char*)calloc(48,sizeof(char));
	char *temp = (char*)calloc(3,sizeof(char));
	Room *r = getRoombyID(headRoom,id);
	str[0] = '0' + NOT_RES;
	str[1] = ' ';
	while(i-2 < strlen(r->player2))
	{
		str[i] = r->player2[i-2];
		i++;
	}
	str[i++] = ' ';
	for(int j = 0; j < 7; j++)
	{
		sprintf(temp,"%d",r->deck[j]);
		p = i;
		while(i-p<strlen(temp))
		{
			str[i] = temp[i - p];
			i++;
		}
		str[i++] = '|';
	}
	str[i-1] = '\0';
	return str;
}
char *joinRoomRes(VALUE_RES res,int id)
{
	int i = 4,j;
	char *str = (char*)calloc(50,sizeof(char));
	char *temp = (char*)calloc(3,sizeof(char));
	Room *r = getRoombyID(headRoom,id);
	str[0] = '0' + JOI_RES;
	str[1] = ' ';
	str[2] = '0' + res;
	if(res !=JOIN_SUCCESS)
	{
		str[3] = '\0';
		return str;
	}
	str[3] = ' ';
	for(int k = 2; k < 9; k++)
	{
		sprintf(temp,"%d",r->deck[k]);
		j = i;
		while(i - j < strlen(temp))
		{
			str[i]  = temp[i - j];
			i++;
		}
		str[i++] = '|';
	}
	str[i-1] = ' ';
	j = i;
	while(i - j < strlen(r->player1))
	{
		str[i] = r->player1[i-j];
		i++;
	}
	str[i] = '\0';
	return str;
}
//if fold :END_RES -1
//else :END_RES 0 card1 card2
char *makeEndRes(int isFold,int id, int mine)
{
	char *str = (char*)calloc(20,sizeof(char));
	char *temp = (char*)calloc(3,sizeof(char));
	int i = 4,j;
	int card1,card2;
	str[0] = '0' + END_RES;
	str[1] = ' ';
	if(isFold == 0)
	{
		str[2] = '-';
		str[3] = '1';
		str[4] = '\0';
		return str;
	}
	Room *r = getRoombyID(headRoom,id);
	str[2] = '0';
	str[3] = ' ';
	if(mine == 1)
	{
		card1 = r->deck[7];
		card2 = r->deck[8];
	}
	else
	{
		card1 = r->deck[0];
		card2 = r->deck[1];
	}
	sprintf(temp,"%d",card1);
	while(i-4 < strlen(temp))
	{
		str[i] = temp[i-4];
		i++;
	}
	str[i++] = ' ';
	j = i;
	sprintf(temp,"%d",card2);
	while(i-j < strlen(temp))
	{
		str[i] = temp[i-j];
		i++;
	}
	str[i] = '\0';
	return str;
}

int main()
{
	memset(Link,0,sizeof(Link));
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sockfd!=-1);
	int ruler;
	char *nickname = (char*)calloc(20,sizeof(char));
	headRoom = makeListRoom();    
	struct sockaddr_in saddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(PORT);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	char clientinfo[MAXFD][20];
	int res=bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	assert(res!=-1);

	listen(sockfd,5);

    fd_set fdset;

    int fds[MAXFD];
    int i=0;
    for(;i<MAXFD;++i)
    {
	  	fds[i]=-1;
    }
	LoadUser();
    fds_add(fds,sockfd);
	while(1)
    {
		FD_ZERO(&fdset);

		int maxfd=-1;

		int i=0;

		for(;i<MAXFD;i++)
		{
			if(fds[i]==-1)
			{
				continue;
			}

			FD_SET(fds[i],&fdset);

			if(fds[i]>maxfd)
			{
				maxfd=fds[i];
			}
		}

		struct timeval tv={120,0};	

		int n=select(maxfd+1,&fdset,NULL,NULL,&tv);
		if(n==-1)	
		{
			perror("select error");
		}
		else if(n==0)
		{
			printf("time out\n");
		}
		else
		{

			for(i=0;i<MAXFD;++i)
			{
				if(fds[i]==-1)	
				{
					continue;
				}
				if(FD_ISSET(fds[i],&fdset))	
				{
			   
					if(fds[i]==sockfd)	
					{
						struct sockaddr_in caddr;
						socklen_t len=sizeof(caddr);

						int c=accept(sockfd,(struct sockaddr *)&caddr,&len);	
						printf("Connect to %s\n",inet_ntoa(caddr.sin_addr));
						if(c<0)
						{
							continue;
						}
					
						fds_add(fds,c);
					}
					else   
					{
						char buff[128]={0};
						int res=recv(fds[i],buff,127,0);
						if(res<=0)
						{
							close(fds[i]);
							fds[i]=-1;
							User *user;
							user = getUserByNickName(headUser,clientinfo[i]);
							user->isLogin = 0;
							ruler = getIdbyIndex(i);
							if(ruler!=0)
							{
								if(i == Link[ruler].i1)
								{
									ruler = Link[ruler].i2;
								}
								else
								{
									ruler = Link[ruler].i1;
								}
								send(fds[ruler],"OUT-GAME",10,0);
							}
						}
						else
						{
							buff[res] = '\0';
							switch(buff[0]-'0')
							{
								case REGISTER:
								{
									ruler = processSignUp(buff,nickname);
									if(ruler == 0)
									{
										send(fds[i],signIURes(REG_RES,EXISTED,nickname),25,0);
									}
									else {
										send(fds[i],signIURes(REG_RES,SUCCESS_SIGNUP,nickname),25,0);
										updateUser();
										strcpy(clientinfo[i],nickname);
									}
									break;
								}
								case LOGIN:
								{
									ruler = processSignIn(buff,nickname);
									if(ruler == 0)
									{
										send(fds[i],signIURes(LOG_RES,NOT_EXIST,nickname),25,0);
									} else if(ruler == 1)
									{
										send(fds[i],signIURes(LOG_RES,WRONG_PASS,nickname),25,0);
									} else if(ruler == -1)
									{
										send(fds[i],signIURes(LOG_RES,LOGED_IN,nickname),25,0);
									}else{
										send(fds[i],signIURes(LOG_RES,SUCCESS_SIGNIN,nickname),25,0);
										strcpy(clientinfo[i],nickname);
									} 
									break;
								}
								case CREATEROOM:
								{
									ruler = processCreateRoom(buff);
									send(fds[i],createRoomRes(ruler),6,0);
									Link[ruler].i1 = i;
									break;
								}
								case GETLIST:
								{
									send(fds[i],listRoomRes(),20,0);
									break;
								}
								case JOINROOM:
								{
									ruler = processJoinRoom(buff);
									if(ruler==0)
										send(fds[i],joinRoomRes(ROOM_NEXIST,ruler),6,0);
									else if(ruler==-1)
										send(fds[i],joinRoomRes(FULL_SLOT,ruler),6,0);
									else if(ruler==-2)
										send(fds[i],joinRoomRes(WRONG_RPASS,ruler),6,0);
									else{
										Link[ruler].i2 = i;
										send(fds[i],joinRoomRes(JOIN_SUCCESS,ruler),50,0);
										send(fds[Link[ruler].i1],noticeRes(ruler),48,0);
									} 
									break;
								}
								case RAISE:
								{
									int i2 = getOpponentIndex(i); 
									send(fds[i2],buff,res+1,0);
									break;
								}
								case ENDGAME:
								{
									int idd = getIdbyIndex(i);
									int i2 = Link[idd].i1 == i ? Link[idd].i2 : Link[idd].i1;
									ruler = processEnd(buff,idd);
									if(ruler == 0)
									{
										send(fds[i2],makeEndRes(ruler,idd,0),20,0);
									}
									else if(ruler == 1)
									{
										if(i2 == Link[idd].i2)
										{
											send(fds[i],makeEndRes(ruler,idd,1),20,0);
											send(fds[i2],makeEndRes(ruler,idd,2),20,0);
										}
										else
										{
											send(fds[i],makeEndRes(ruler,idd,2),20,0);
											send(fds[i2],makeEndRes(ruler,idd,1),20,0);
										}
										
									}
									Link[idd].i1 = 0;
									Link[idd].i2 = 0; 
									removeRoom(&headRoom,idd);
									break;
								}
								case PLUS:
								{
									processPlusScore(buff);
									updateUser();
									break;
								}
								case GETSCORE:
								{
									ruler = processGetScore(buff);
									send(fds[i],scoreMessage(ruler),10,0);
									break;
								}
								case LOGOUT:
								{
									processLogOut(buff);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}