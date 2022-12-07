#pragma once
#define ROOM_NTY_BUF_SIZE 1024
#include <iostream>// memset, strlen 사용 하기 위해 필요
class S2C_ROOM_ENTER_NTY
{
private:
	int totalBufferSize;
	int code;
	int result;
	char msg[ROOM_NTY_BUF_SIZE] = { 0, };
	int userIndex;
public:
	char* Serialize(int _totalBufferSize, int _code, int _result, int userIndex, char _msg[ROOM_NTY_BUF_SIZE] = { 0, });
	void Deserialize(char* _String);
	int GetTotalBufferSize();
	int GetCode();
	int GetResult();
	char* GetMsg();
	int GetUserIndex();
};