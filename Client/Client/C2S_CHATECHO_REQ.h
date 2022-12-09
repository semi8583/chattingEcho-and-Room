#pragma once
#define ECHO_REQ_BUF_SIZE 1024
#include <iostream>// memset, strlen ��� �ϱ� ���� �ʿ�
class C2S_CHATECHO_REQ
{
private:
	int size;
	int code = 0;
	int stringLength;
	int userIdx;
	char msg[ECHO_REQ_BUF_SIZE] = { 0, };
public:
	char* Serialize(int _size, int _code, int _stringLength, int _userIdx, char* _msg);
	void Deserialize(char* _inputString);

	int GetSize();
	int GetCode();
	int GetStringLength();
	char* GetMsg();
	int GetUserIdx();
};