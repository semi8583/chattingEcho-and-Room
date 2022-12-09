#include "C2S_ROOM_ENTER_REQ.h"
char* C2S_ROOM_ENTER_REQ::Serialize(int _size, int _code, int _roomNo, int _userIdx)
{
	memset(this->msg, 0, ROOM_REQ_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&(this->msg[0]), &_size, sizeof(_size));
	memcpy(&(this->msg[4]), &_code, sizeof(_code));
	memcpy(&(this->msg[8]), &_roomNo, sizeof(_roomNo));
	memcpy(&(this->msg[12]), &_userIdx, sizeof(_userIdx));

	return this->msg;
}

void C2S_ROOM_ENTER_REQ::Deserialize(char* _String)
{
	memset(this->msg, 0, ROOM_REQ_BUF_SIZE); // 기존 멤버 변수 초기화

	memcpy(&this->size, &(_String[0]), sizeof(int));
	memcpy(&this->code, &(_String[4]), sizeof(int));
	memcpy(&this->roomNo, &(_String[8]), sizeof(int));
	memcpy(&this->userIdx, &(_String[12]), sizeof(int));
	for (int i = 0; i < this->size; i++)
		this->msg[i] = _String[i];
}

int C2S_ROOM_ENTER_REQ::GetSize()
{
	return this->size;
}

int C2S_ROOM_ENTER_REQ::GetCode()
{
	return this->code;
}

int C2S_ROOM_ENTER_REQ::GetRoomNo()
{
	return this->roomNo;
}

void C2S_ROOM_ENTER_REQ::SetRoomNo(int _roomNo)
{
	this->roomNo = _roomNo;
	memcpy(&this->msg[8], &(_roomNo), sizeof(int));
}

int C2S_ROOM_ENTER_REQ::GetUserIdx()
{
	return this->userIdx;
}

char* C2S_ROOM_ENTER_REQ::GetMsg()
{
	return this->msg;
}