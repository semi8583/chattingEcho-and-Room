#include "S2C_CHATECHO_ACK.h"

char* S2C_CHATECHO_ACK::Serialize(int _size, int _code, int _result, int _stringLength, char* _msg)
{
	memset(this->msg, 0, ECHO_ACK_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_size, sizeof(_size));
	memcpy(&(this->msg[4]), &_code, sizeof(_code));
	memcpy(&(this->msg[8]), &_result, sizeof(_result));
	memcpy(&(this->msg[12]), &_stringLength, sizeof(_stringLength));

	for (int i = 0; i < _stringLength; i++)
		this->msg[16 + i] = _msg[i];

	return this->msg;
}
void S2C_CHATECHO_ACK::Deserialize(char* _inputString)
{
	memset(this->msg, 0, ECHO_ACK_BUF_SIZE); // 기존 멤버 변수 초기화
	memcpy(&this->size, &(_inputString[0]), sizeof(int));
	memcpy(&this->code, &(_inputString[4]), sizeof(int));
	memcpy(&this->result, &(_inputString[8]), sizeof(int));
	memcpy(&this->stringLength, &(_inputString[12]), sizeof(int));

	for (int j = 0; j < this->stringLength; j++)
		this->msg[j] = _inputString[j + 16];
}
int S2C_CHATECHO_ACK::GetSize()
{
	return this->size;
}
int S2C_CHATECHO_ACK::GetCode()
{
	return this->code;
}
int S2C_CHATECHO_ACK::GetResult()
{
	return this->result;
}
void S2C_CHATECHO_ACK::SetResult(int _result)
{
	this->result = _result;
	memcpy(&(this->msg[8]), &this->result, sizeof(int));
}
int S2C_CHATECHO_ACK::GetStringLength()
{
	return this->stringLength;
}
char* S2C_CHATECHO_ACK::GetMsg()
{
	return this->msg;
}