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
#define MAXFD 10	//Size of fds array
#define PORT 6666	
User* headUser = NULL;
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
int processSignUpRequest(char *msg)
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
	pushUser(headUser,username,password,nickname);
	return 1;
}
int processSignInRequest(char *msg)
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
		return 1;
	}
	return 2;
}
char *makeResMessage(RES_TYPE type,SIGNIU_RES res)
{
	char *msg = (char*)calloc(4,sizeof(char));
	msg[0] = '0' + type;
	msg[1] = ' ';
	msg[2] = '0' + res;
	msg[3] = '\0';
	return msg;
}
int main()
{
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sockfd!=-1);
	int ruler;
	
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
							switch(buff[0]-'0')
							{
								case REGISTER:
								{
									ruler = processSignUpRequest(buff);
									if(ruler == 0)
									{
										send(fds[i],makeResMessage(REG_RES,EXISTED),5,0);
									}
									else send(fds[i],makeResMessage(REG_RES,SUCCESS_SIGNUP),5,0);
									break;
								}
								case LOGIN:
								{
									ruler = processSignInRequest(buff);
									if(ruler == 0)
									{
										send(fds[i],makeResMessage(LOG_RES,NOT_EXIST),5,0);
									} else if(ruler == 1)
									{
										send(fds[i],makeResMessage(LOG_RES,WRONG_PASS),5,0);
									}else send(fds[i],makeResMessage(LOG_RES,SUCCESS_SIGNIN),5,0);
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