#include "C2S_CHATECO_REQ.h"
#include <iostream>
using namespace std;
char* C2S_CHATECO_REQ::Serialize(int _totalStringLength, int _inpuStringLength, char* _inputString)
{
	memset(this->msg, 0, ECO_REQ_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_totalStringLength, sizeof(_totalStringLength));
	memcpy(&(this->msg[4]), &_inpuStringLength, sizeof(_inpuStringLength));
	
	for (int i = 0; i < _inpuStringLength; i++)
		this->msg[8 + i] = _inputString[i];
	
	return this->msg;
}

void C2S_CHATECO_REQ::Deserialize(char* _inputString)
{
	memset(this->msg, 0, ECO_REQ_BUF_SIZE); // 기존 멤버 변수 초기화
	memcpy(&this->totalBufferSize, &(_inputString[0]), sizeof(int));
	memcpy(&this->inputStringLength, &(_inputString[4]), sizeof(int));

	for (int j = 0; j < inputStringLength; j++)
		this->msg[j] = _inputString[j + 8];
}

int C2S_CHATECO_REQ::GetTotalBufferSize()
{
	return this->totalBufferSize;
}

int C2S_CHATECO_REQ::GetInputStringLength()
{
	return this->inputStringLength;
}

char* C2S_CHATECO_REQ::GetMsg()
{
	return this->msg;
}