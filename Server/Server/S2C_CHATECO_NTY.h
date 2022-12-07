#pragma once
#define ECO_NTY_BUF_SIZE 1024
#include <iostream>// memset, strlen 사용 하기 위해 필요
class S2C_CHATECO_NTY
{
private:
	int totalBufferSize;
	int inputStringLength;
	char msg[ECO_NTY_BUF_SIZE] = { 0, };
	int userIndex;
public:
	char* Serialize(int _totalStringLength, int _inputStringLength, char* _inputString, int _userIndex);
	void Deserialize(char* _inputString);

	int GetTotalBufferSize();
	int GetInputStringLength();
	char* GetMsg();
	int GetUserIndex();
};