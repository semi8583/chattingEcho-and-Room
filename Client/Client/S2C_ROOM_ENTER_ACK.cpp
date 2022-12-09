#include "S2C_ROOM_ENTER_ACK.h"
char* S2C_ROOM_ENTER_ACK::Serialize(int _size, int _code, int _roomNo, int _result)
{
	memset(this->msg, 0, ROOM_ACK_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_size, sizeof(_size));
	memcpy(&(this->msg[4]), &_code, sizeof(_code));
	memcpy(&(this->msg[8]), &_roomNo, sizeof(_roomNo));
	memcpy(&(this->msg[12]), &_result, sizeof(_result));

	return this->msg;
}
void S2C_ROOM_ENTER_ACK::Deserialize(char* _string)
{
	memset(this->msg, 0, ROOM_ACK_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&this->size, &(_string[0]), sizeof(int));
	memcpy(&this->code, &(_string[4]), sizeof(int));
	memcpy(&this->roomNo, &(_string[8]), sizeof(int));
	memcpy(&this->result, &(_string[12]), sizeof(int));
	for (int i = 0; i < this->size; i++)
		this->msg[i] = _string[i];
}

int S2C_ROOM_ENTER_ACK::GetSize()
{
	return this->size;
}
int S2C_ROOM_ENTER_ACK::GetCode()
{
	return this->code;
}
int S2C_ROOM_ENTER_ACK::GetRoomNo()
{
	return this->roomNo;
}
int S2C_ROOM_ENTER_ACK::GetResult()
{
	return this->result;
}
void S2C_ROOM_ENTER_ACK::SetResult(int _result)
{
	this->result = _result;
	memcpy(&(this->msg[12]), &this->result, sizeof(_result));
}
char* S2C_ROOM_ENTER_ACK::GetMsg()
{
	return this->msg;
}