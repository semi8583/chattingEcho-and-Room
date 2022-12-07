#pragma once
#define ECO_REQ_BUF_SIZE 1024
#include <iostream>// memset, strlen 사용 하기 위해 필요
class C2S_CHATECO_REQ
{
private:
	int totalBufferSize;
	int inputStringLength;
	char msg[ECO_REQ_BUF_SIZE] = { 0, };
public:
	char* Serialize(int _totalStringLength, int _inpuStringLength, char* _inputString);
	void Deserialize(char* _inputString);
	
	int GetTotalBufferSize();
	int GetInputStringLength();
	char* GetMsg();
};