#include "C2S_PID_REQ.h"
char *C2S_PID_REQ::Serialize(int _size, int _code, int _pid)
{
	memset(this->msg, 0, 12); // 기존 멤버 변수 초기화
	memcpy(&(this->msg[0]), &_size, sizeof(_size));
	memcpy(&(this->msg[4]), &_code, sizeof(_code));
	memcpy(&(this->msg[8]), &_pid, sizeof(_pid));
	return this->msg;
}

void C2S_PID_REQ::Deserialize(char* _inputString)
{
	memset(this->msg, 0, 12); // 기존 멤버 변수 초기화

	memcpy(&this->size, &(_inputString[0]), sizeof(int));
	memcpy(&this->code, &(_inputString[4]), sizeof(int));
	memcpy(&this->pid, &(_inputString[8]), sizeof(int));
	for (int i = 0; i < this->size; i++)
		this->msg[i] = _inputString[i];
}
int C2S_PID_REQ::GetSize()
{
	return this->size;
}
int C2S_PID_REQ::GetCode()
{
	return this->code;
}
int C2S_PID_REQ::GetPid()
{
	return this->pid;
}
char* C2S_PID_REQ::GetMsg()
{
	return this->msg;
}