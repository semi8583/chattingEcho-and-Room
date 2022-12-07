#pragma once
#define ROOM_ACK_BUF_SIZE 1024
#include <iostream>// memset, strlen 사용 하기 위해 필요
class S2C_ROOM_ENTER_ACK
{
private:
	int totalBufferSize;
	int code;
	int result;
	char msg[ROOM_ACK_BUF_SIZE] = { 0, };
public:
	char* Serialize(int _totalBufferSize, int _code, int _result);
	void Deserialize(char* _String);

	int GetTotalBufferSize();
	int GetCode();
	int GetResult();
	void SetResult(int _result);
	char* GetMsg();
};

