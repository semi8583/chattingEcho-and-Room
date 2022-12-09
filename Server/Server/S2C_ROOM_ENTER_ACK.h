#pragma once
#define ROOM_ACK_BUF_SIZE 1024
#include <iostream>// memset, strlen 사용 하기 위해 필요
class S2C_ROOM_ENTER_ACK
{
private:
	int size;
	int code = 0;
	int roomNo = 0;
	int result;
	char msg[ROOM_ACK_BUF_SIZE] = { 0, };
public:
	char* Serialize(int _size, int _code, int _roomNo, int _result);
	void Deserialize(char* _string);

	int GetSize();
	int GetCode();
	int GetRoomNo();
	int GetResult();
	void SetResult(int _result);
	char* GetMsg();
};