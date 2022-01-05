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
	headUser = makeList();
	while(!feof(file))
	{
		fscanf(file,"%s %s %s\n",username,password,nickname);
		if(feof(file))
			break;
		pushUser(headUser,username,password,nickname);
	}
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
int processSignUpRequest(char *msg,char *nick)
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
	pushUser(headUser,username,password,nickname);
	return 1;
}
int processSignInRequest(char *msg,char *nick)
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
	strcpy(nick,loginUser->nickname);
	return 2;
}
char *signIUResMessage(RES_TYPE type,VALUE_RES res,char *nickname)
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
int processBetRequest(char *msg)
{
	return 0;
}
char *betResMessage()
{
	//TODO
	return "alo";
}
char *listRoomMessage()
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
char *createRoomResMessage(int ruler)
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
int processCreateRoomRequest(char *msg)
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
	Player p = newPlayer(nickname,100);
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
		printf("%s\n",password );
		id = pushRoom(&headRoom,1,password,p);
		return id;
	}
	strcpy(password,"\0");
	id = pushRoom(&headRoom,0,password,p);
	return id;
}

int processJoinRoomRequest(char *msg)
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
	Player p = newPlayer(nickname,100);
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
			joinRoom(r,p);
			setDeckToRoom(r);
			return id;
		}
		else
		{
			return -2;
		}
	}
	joinRoom(r,p);
	setDeckToRoom(r);
	return id;
}
char *makeJoinNoticeToCreatorMessage(int id)
{
	int i = 2,p;
	char *str = (char*)calloc(48,sizeof(char));
	char *temp = (char*)calloc(3,sizeof(char));
	Room *r = getRoombyID(headRoom,id);
	str[0] = '0' + NOT_RES;
	str[1] = ' ';
	while(i-2 < strlen(r->player[1].nickname))
	{
		str[i] = r->player[1].nickname[i-2];
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
char *makeJoinRoomResMessage(VALUE_RES res,int id)
{
	int i = 4,j;
	char *str = (char*)calloc(30,sizeof(char));
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
	str[i-1] = '\0';
	return str;
}
int main()
{
	memset(Link,0,sizeof(Link));
	Player p = newPlayer("ngocbao",100);
	Player q = newPlayer("bao",100);
	Player e = newPlayer("an",100);
	pushRoom(&headRoom,0,"",p);
	pushRoom(&headRoom,0,"",q);
	pushRoom(&headRoom,1,"abc",e);
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sockfd!=-1);
	int ruler;
	int *result = (int*)calloc(3,sizeof(int));
	char *nickname = (char*)calloc(20,sizeof(char));
	
    printf("sockfd=%d\n",sockfd);
    
	struct sockaddr_in saddr,caddr;
	memset(&saddr,0,sizeof(saddr));
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(PORT);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

	int res=bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	assert(res!=-1);
	
	//Create listening queue
	listen(sockfd,5);
	
   //Define fdset collection
    fd_set fdset;
	
	//Define fds array
    int fds[MAXFD];
    int i=0;
    for(;i<MAXFD;++i)
    {
	  	fds[i]=-1;
    }
	
	//Add a file descriptor to the fds array
    fds_add(fds,sockfd);
	while(1)
    {
		FD_ZERO(&fdset);//Clear the fdset array to 0

		int maxfd=-1;

		int i=0;

		//For loop finds the maximum subscript for the ready event in the fds array
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

		struct timeval tv={120,0};	//Set timeout of 2 minute

		int n=select(maxfd+1,&fdset,NULL,NULL,&tv);//Selectect system call, where we only focus on read events
		if(n==-1)	//fail
		{
			perror("select error");
		}
		else if(n==0)//Timeout, meaning no file descriptor returned
		{
			printf("time out\n");
		}
		else//Ready event generation
		{
		//Because we only know the number of ready events by the return value of select, we don't know which events are ready.
		//Therefore, each file descriptor needs to be traversed for judgment
			for(i=0;i<MAXFD;++i)
			{
				if(fds[i]==-1)	//If fds[i]==-1, the event is not ready
				{
					continue;
				}
				if(FD_ISSET(fds[i],&fdset))	//Determine if the event corresponding to the file descriptor is ready
				{
			   
				//There are two kinds of cases for judging file descriptors
			   
					if(fds[i]==sockfd)	//A file descriptor is a socket, meaning accept if a new client requests a connection
					{
						//accept
						struct sockaddr_in caddr;
						int len=sizeof(caddr);

						int c=accept(sockfd,(struct sockaddr *)&caddr,&len);	//Accept new client connections
						printf("Connect to %s\n",inet_ntoa(caddr.sin_addr));
						if(c<0)
						{
							continue;
						}
					
						printf("accept c=%d\n",c);
						fds_add(fds,c);//Add the connection socket to the array where the file descriptor is stored
					}
					else   //Receive data recv when an existing client sends data
					{
						char buff[128]={0};
						LoadUser();
						int res=recv(fds[i],buff,127,0);
						if(res<=0)
						{
							close(fds[i]);
							fds[i]=-1;
							printf("one client over\n");
						}
						else
						{
							buff[res] = '\0';
							switch(buff[0]-'0')
							{
								case REGISTER:
								{
									ruler = processSignUpRequest(buff,nickname);
									if(ruler == 0)
									{
										send(fds[i],signIUResMessage(REG_RES,EXISTED,nickname),25,0);
									}
									else {
										send(fds[i],signIUResMessage(REG_RES,SUCCESS_SIGNUP,nickname),25,0);
									}
									break;
								}
								case LOGIN:
								{
									ruler = processSignInRequest(buff,nickname);
									if(ruler == 0)
									{
										send(fds[i],signIUResMessage(LOG_RES,NOT_EXIST,nickname),25,0);
									} else if(ruler == 1)
									{
										send(fds[i],signIUResMessage(LOG_RES,WRONG_PASS,nickname),25,0);
									}else send(fds[i],signIUResMessage(LOG_RES,SUCCESS_SIGNIN,nickname),25,0);
									printf("%d\n",i );
									break;
								}
								case CREATEROOM:
								{
									ruler = processCreateRoomRequest(buff);
									send(fds[i],createRoomResMessage(ruler),6,0);
									Link[ruler].i1 = i;
									break;
								}
								case GETLIST:
								{
									send(fds[i],listRoomMessage(),20,0);
									break;
								}
								case JOINROOM:
								{
									ruler = processJoinRoomRequest(buff);
									if(ruler==0)
										send(fds[i],makeJoinRoomResMessage(ROOM_NEXIST,ruler),6,0);
									else if(ruler==-1)
										send(fds[i],makeJoinRoomResMessage(FULL_SLOT,ruler),6,0);
									else if(ruler==-2)
										send(fds[i],makeJoinRoomResMessage(WRONG_RPASS,ruler),6,0);
									else{
										Link[ruler].i2 = i;
										send(fds[i],makeJoinRoomResMessage(JOIN_SUCCESS,ruler),30,0);
										send(fds[Link[ruler].i1],makeJoinNoticeToCreatorMessage(ruler),48,0);
										printf("%d-%d\n",Link[ruler].i1,Link[ruler].i2);
									} 
									break;
								}
								case BET:
								{
									ruler = processBetRequest(buff);
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