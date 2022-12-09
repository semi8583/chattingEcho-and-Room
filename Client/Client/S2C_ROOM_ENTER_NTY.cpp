#include "S2C_ROOM_ENTER_NTY.h"
char* S2C_ROOM_ENTER_NTY::Serialize(int _size, int _code, int _roomNo, int _userIdx)
{
	memset(this->msg, 0, ROOM_NTY_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_size, sizeof(_size));
	memcpy(&(this->msg[4]), &_code, sizeof(_code));
	memcpy(&(this->msg[8]), &_roomNo, sizeof(_roomNo));
	memcpy(&(this->msg[12]), &_userIdx, sizeof(_userIdx));

	return this->msg;
}
void S2C_ROOM_ENTER_NTY::Deserialize(char* _String)
{
	memset(this->msg, 0, ROOM_NTY_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&this->size, &(_String[0]), sizeof(int));
	memcpy(&this->code, &(_String[4]), sizeof(int));
	memcpy(&this->roomNo, &(_String[8]), sizeof(int));
	memcpy(&this->userIdx, &(_String[12]), sizeof(int));
	for (int i = 0; i < this->size; i++)
		this->msg[i] = _String[i];
}
int S2C_ROOM_ENTER_NTY::GetSize()
{
	return this->size;
}
int S2C_ROOM_ENTER_NTY::GetCode()
{
	return this->code;
}
int S2C_ROOM_ENTER_NTY::GetRoomNo()
{
	return this->roomNo;
}
int S2C_ROOM_ENTER_NTY::GetUserIdx()
{
	return this->userIdx;
}
char* S2C_ROOM_ENTER_NTY::GetMsg()
{
	return this->msg;
}