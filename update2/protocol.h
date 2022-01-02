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
	GETLIST,
	JOINROOM,
	LOGOUT,
	BET,
}MSG_TYPE;

typedef enum{
	REG_RES,
	LOG_RES,
	CRE_RES,
	GET_RES,
	JOI_RES,
	BET_RES
}RES_TYPE;

typedef enum{
	SUCCESS_SIGNIN,//dang nhap thanh cong
	NOT_EXIST,//tai khoan khong ton tai
	WRONG_PASS,//sai mat khau
	SUCCESS_SIGNUP,//dang ky thanh cong
	EXISTED,//tai khoan da ton tai
	WRONG_RPASS,// sai pass room
	ROOM_NEXIST,//phong khong ton tai
	FULL_SLOT,//phong day
	JOIN_SUCCESS,//vao phong thanh cong
}VALUE_RES;


char *makeSignInMessage();
char *makeSignUpMessage();
char *makeCreateRoomMessage(char* nickname);
char *getAvailableRoomMessage();
char *makeJoinRoomMessage(char *nickname);
#endif