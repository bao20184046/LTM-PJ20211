#ifndef __PROTOCOL__
#define __PROTOCOL__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MSG_SIZE 1024

typedef enum{
	REGISTER,
	LOGIN,
	CREATEROOM,
	JOINROOM,
	LOGOUT,
	BET,
}MSG_TYPE;

typedef enum{
	REG_RES,
	LOG_RES,
	CRE_RES,
	JOI_RES,
	BET_RES
}RES_TYPE;

typedef enum{
	SUCCESS_SIGNIN,
	NOT_EXIST,
	WRONG_PASS,
	SUCCESS_SIGNUP,
	EXISTED,
}SIGNIU_RES;


char *makeSignInMessage();
char *makeSignUpMessage();

#endif