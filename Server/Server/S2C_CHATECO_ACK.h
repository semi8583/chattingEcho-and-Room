#pragma once
#define ECO_ACK_BUF_SIZE 1024
#include <iostream>// memset, strlen ��� �ϱ� ���� �ʿ�
class S2C_CHATECO_ACK
{
private:
	int totalBufferSize;
	int code = 0; // 0: ���ڿ� �Է� ����
	int result;
	int inputStringLength;
	char msg[ECO_ACK_BUF_SIZE] = { 0, };
public:
	char* Serialize(int _totalStringLength, int _code, int _result, int _inputStringLength, char* _inputString);
	void Deserialize(char* _inputString);

	int GetTotalBufferSize();
	int GetCode();
	void SetCode(int _code);
	int GetResult();
	void SetResult(int _result);
	int GetInputStringLength();
	char* GetMsg();
};