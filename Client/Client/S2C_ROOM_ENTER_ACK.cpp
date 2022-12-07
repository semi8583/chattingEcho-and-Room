#include "S2C_ROOM_ENTER_ACK.h"
char* S2C_ROOM_ENTER_ACK::Serialize(int _totalBufferSize, int _code, int _result)
{
	memset(this->msg, 0, ROOM_ACK_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_totalBufferSize, sizeof(_totalBufferSize));
	memcpy(&(this->msg[4]), &_code, sizeof(char));
	memcpy(&(this->msg[5]), &_result, sizeof(char));

	return this->msg;
}
void S2C_ROOM_ENTER_ACK::Deserialize(char* _String)
{
	memset(this->msg, 0, ROOM_ACK_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&this->totalBufferSize, &(_String[0]), sizeof(int));
	memcpy(&this->code, &(_String[4]), sizeof(char));
	memcpy(&this->result, &(_String[5]), sizeof(char));
	memcpy(&this->msg, &(_String[0]), 6);
}

int S2C_ROOM_ENTER_ACK::GetTotalBufferSize()
{
	return this->totalBufferSize;
}
int S2C_ROOM_ENTER_ACK::GetCode()
{
	return this->code;
}
int S2C_ROOM_ENTER_ACK::GetResult()
{
	return this->result;
}
void S2C_ROOM_ENTER_ACK::SetResult(int _result)
{
	this->result = _result;
	memcpy(&(this->msg[5]), &this->result, sizeof(char));
}
char* S2C_ROOM_ENTER_ACK::GetMsg()
{
	return this->msg;
}