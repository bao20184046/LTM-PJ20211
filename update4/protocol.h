#ifndef __PROTOCOL__
#define __PROTOCOL__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#define MSG_SIZE 1024
#define ROUND 1;
#define BET 0;
#define CHIP 100;
typedef enum{
	REGISTER,
	LOGIN,
	CREATEROOM,
	GETLIST,
	JOINROOM,
	RAISE,
	ENDGAME,
	PLUS,
	GETSCORE
}MSG_TYPE;

typedef enum{
	REG_RES,
	LOG_RES,
	CRE_RES,
	GET_RES,
	JOI_RES,
	NOT_RES,
	BET_RES,
	END_RES,
	SCORE_RES
}RES_TYPE;

typedef enum{
	SUCCESS_SIGNIN,//dang nhap thanh cong
	NOT_EXIST,//tai khoan khong ton tai
	WRONG_PASS,//sai mat khau
	LOGED_IN,//dang nhap o noi khac roi
	SUCCESS_SIGNUP,//dang ky thanh cong
	EXISTED,//tai khoan da ton tai
	WRONG_RPASS,// sai pass room
	ROOM_NEXIST,//phong khong ton tai
	FULL_SLOT,//phong day
	JOIN_SUCCESS,//vao phong thanh cong
}VALUE_RES;

typedef enum{
	FOLD,//dau hang
	ALLIN,//2 nguoi all in
	COMMON//cuoc hoan tat
}ENDGAME_TYPE;
char *makeSignInMessage();
char *makeSignUpMessage();
char *makeCreateRoomMessage(char* nickname);
char *getAvailableRoomMessage();
char *makeJoinRoomMessage(char *nickname);
char *makeBetMessage(int newbet);
char *makeNewRoundBetMessage(int newbet);
char *makeEndMessage(ENDGAME_TYPE type);
char *getScoreMessage(char *nickname);
char *makePlusScoreMessage(char *nickname, int score);
#endif