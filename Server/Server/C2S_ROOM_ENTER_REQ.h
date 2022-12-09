#pragma once
#define ROOM_REQ_BUF_SIZE 1024
#include <iostream>// memset, strlen 사용 하기 위해 필요
class C2S_ROOM_ENTER_REQ
{
private:
	int size;
	int code = 0;
	int roomNo = 0;
	int userIdx;
	char msg[ROOM_REQ_BUF_SIZE] = { 0, };
public:
	char* Serialize(int _size, int _code, int _roomNo, int _userIdx);
	void Deserialize(char* _String);

	int GetSize();
	int GetCode();
	int GetRoomNo();
	void SetRoomNo(int _roomNo);
	int GetUserIdx();
	char* GetMsg();
};