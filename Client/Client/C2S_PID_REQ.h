#pragma once
#include <iostream>// memset, strlen 사용 하기 위해 필요
class C2S_PID_REQ
{
private:
	int size;
	int code = 0;
	int pid;
	char msg[4] = { 0, };
public:
	char* Serialize(int _size, int _code, int _pid);
	void Deserialize(char* _inputString);
	int GetSize();
	int GetCode();
	int GetPid();
	char* GetMsg();
};

