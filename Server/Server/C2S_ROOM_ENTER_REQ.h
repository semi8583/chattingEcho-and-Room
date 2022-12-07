#pragma once
#define ROOM_REQ_BUF_SIZE 1024
#include <iostream>// memset, strlen 사용 하기 위해 필요
class C2S_ROOM_ENTER_REQ
{
private:
	int totalBufferSize;
	int code;
	int result;
	char msg[ROOM_REQ_BUF_SIZE] = {0, };
public:
	char* Serialize(int _totalBufferSize, int _code, int _result);
	void Deserialize(char* _String);

	int GetTotalBufferSize();
	int GetCode();
	void SetCode(int _code);
	int GetResult();
	char* GetMsg();
};

