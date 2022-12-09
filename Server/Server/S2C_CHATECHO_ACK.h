#pragma once
#define ECHO_ACK_BUF_SIZE 1024
#include <iostream>// memset, strlen 사용 하기 위해 필요
class S2C_CHATECHO_ACK
{
private:
	int size;
	int code = 0;
	int result;
	int stringLength;
	char msg[ECHO_ACK_BUF_SIZE] = { 0, };
public:
	char* Serialize(int _size, int _code, int _result, int _stringLength, char* _msg);
	void Deserialize(char* _inputString);

	int GetSize();
	int GetCode();
	int GetResult();
	void SetResult(int _result);
	int GetStringLength();
	char* GetMsg();
};