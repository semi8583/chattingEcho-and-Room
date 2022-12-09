#include "C2S_CHATECHO_REQ.h"
#include <iostream>
using namespace std;
char* C2S_CHATECHO_REQ::Serialize(int _size, int _code, int _stringLength, int _userIdx, char* _msg)
{
	memset(this->msg, 0, ECHO_REQ_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_size, sizeof(_size));
	memcpy(&(this->msg[4]), &_code, sizeof(_code));
	memcpy(&(this->msg[8]), &_stringLength, sizeof(_stringLength));
	memcpy(&(this->msg[12]), &_userIdx, sizeof(_userIdx));

	for (int i = 0; i < _stringLength; i++)
		this->msg[16 + i] = _msg[i];

	return this->msg;
}

void C2S_CHATECHO_REQ::Deserialize(char* _inputString)
{
	memset(this->msg, 0, ECHO_REQ_BUF_SIZE); // 기존 멤버 변수 초기화
	memcpy(&this->size, &(_inputString[0]), sizeof(int));
	memcpy(&this->code, &(_inputString[4]), sizeof(int));
	memcpy(&this->stringLength, &(_inputString[8]), sizeof(int));
	memcpy(&(this->userIdx), &(_inputString[12]), sizeof(int));

	for (int j = 0; j < this->stringLength; j++)
		this->msg[j] = _inputString[j + 16];
}

int C2S_CHATECHO_REQ::GetSize()
{
	return this->size;
}

int C2S_CHATECHO_REQ::GetCode()
{
	return this->code;
}
int C2S_CHATECHO_REQ::GetStringLength()
{
	return this->stringLength;
}
char* C2S_CHATECHO_REQ::GetMsg()
{
	return this->msg;
}
int C2S_CHATECHO_REQ::GetUserIdx()
{
	return this->userIdx;
}