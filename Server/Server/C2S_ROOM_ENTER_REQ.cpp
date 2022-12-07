#include "C2S_ROOM_ENTER_REQ.h"
char* C2S_ROOM_ENTER_REQ::Serialize(int _totalBufferSize, int _code, int _result)
{
	memset(this->msg, 0, ROOM_REQ_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_totalBufferSize, sizeof(_totalBufferSize));
	memcpy(&(this->msg[4]), &_code, sizeof(char));
	memcpy(&(this->msg[5]), &_result, sizeof(char));

	return this->msg;
}

void C2S_ROOM_ENTER_REQ::Deserialize(char* _String)
{
	memset(this->msg, 0, ROOM_REQ_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&this->totalBufferSize, &(_String[0]), sizeof(int));
	memcpy(&this->code, &(_String[4]), sizeof(char));
	memcpy(&this->result, &(_String[5]), sizeof(char));
	memcpy(&this->msg, &(_String[0]), sizeof(_String));
}

int C2S_ROOM_ENTER_REQ::GetTotalBufferSize()
{
	return this->totalBufferSize;
}

int C2S_ROOM_ENTER_REQ::GetCode()
{
	return this->code;
}

void C2S_ROOM_ENTER_REQ::SetCode(int _code)
{
	this->code = _code;
	memcpy(&(this->msg[4]), &this->code, sizeof(char));
}

int C2S_ROOM_ENTER_REQ::GetResult()
{
	return this->result;
}

char* C2S_ROOM_ENTER_REQ::GetMsg()
{
	return this->msg;
}