#include "S2C_ROOM_ENTER_NTY.h"
char* S2C_ROOM_ENTER_NTY::Serialize(int _totalBufferSize, int _code, int _result, int userIndex, char _msg[ROOM_NTY_BUF_SIZE])
{
	memset(this->msg, 0, ROOM_NTY_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_totalBufferSize, sizeof(_totalBufferSize));
	memcpy(&(this->msg[4]), &_code, sizeof(char));
	memcpy(&(this->msg[5]), &_result, sizeof(char));
	memcpy(&(this->msg[6]), &userIndex, sizeof(userIndex));

	return this->msg;
}
void S2C_ROOM_ENTER_NTY::Deserialize(char* _String)
{
	memset(this->msg, 0, ROOM_NTY_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&this->totalBufferSize, &(_String[0]), sizeof(int));
	this->code = _String[4];
	this->result = _String[5];
	memcpy(&this->msg, &(_String[0]), sizeof(_String));
	memcpy(&this->userIndex, &(_String[6]), sizeof(this->userIndex));
}
int S2C_ROOM_ENTER_NTY::GetTotalBufferSize()
{
	return this->totalBufferSize;
}
int S2C_ROOM_ENTER_NTY::GetCode()
{
	return this->code;
}
int S2C_ROOM_ENTER_NTY::GetResult()
{
	return this->result;
}
char* S2C_ROOM_ENTER_NTY::GetMsg()
{
	return this->msg;
}
int S2C_ROOM_ENTER_NTY::GetUserIndex()
{
	return this->userIndex;
}