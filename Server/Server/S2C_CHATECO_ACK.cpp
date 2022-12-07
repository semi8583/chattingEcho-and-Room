#include "S2C_CHATECO_ACK.h"

char* S2C_CHATECO_ACK::Serialize(int _totalStringLength, int _code, int _result, int _inputStringLength, char* _inputString)
{
	memset(this->msg, 0, ECO_ACK_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_totalStringLength, sizeof(_totalStringLength));
	memcpy(&(this->msg[4]), &_code, sizeof(char));
	memcpy(&(this->msg[5]), &_result, sizeof(char));
	memcpy(&(this->msg[6]), &_inputStringLength, sizeof(_inputStringLength));

	for (int i = 0; i < _inputStringLength; i++)
		this->msg[10 + i] = _inputString[i];

	return this->msg;
}
void S2C_CHATECO_ACK::Deserialize(char* _inputString)
{
	memset(this->msg, 0, ECO_ACK_BUF_SIZE); // 기존 멤버 변수 초기화
	memcpy(&this->totalBufferSize, &(_inputString[0]), sizeof(int));
	memcpy(&this->code, &(_inputString[4]), sizeof(char));
	memcpy(&this->result, &(_inputString[5]), sizeof(char));
	memcpy(&this->inputStringLength, &(_inputString[6]), sizeof(int));

	for (int j = 0; j < inputStringLength; j++)
		this->msg[j] = _inputString[j + 10];
}
int S2C_CHATECO_ACK::GetTotalBufferSize()
{
	return this->totalBufferSize;
}
int S2C_CHATECO_ACK::GetCode()
{
	return this->code;
}
void S2C_CHATECO_ACK::SetCode(int _code)
{
	this->code = _code;
	memcpy(&(this->msg[4]), &this->code, sizeof(char));
}
int S2C_CHATECO_ACK::GetResult()
{
	return this->result;
}
void S2C_CHATECO_ACK::SetResult(int _result)
{
	this->result = _result;
	memcpy(&(this->msg[5]), &this->result, sizeof(char));
}
int S2C_CHATECO_ACK::GetInputStringLength()
{
	return this->inputStringLength;
}
char* S2C_CHATECO_ACK::GetMsg()
{
	return this->msg;
}