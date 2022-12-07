#include "S2C_CHATECO_NTY.h"
char* S2C_CHATECO_NTY::Serialize(int _totalStringLength, int _inputStringLength, char* _inputString, int _userIndex)
{
    memset(this->msg, 0, ECO_NTY_BUF_SIZE); // 기존 멤버 변수 초기화

    memcpy(&(this->msg[0]), &_totalStringLength, sizeof(_totalStringLength));
    memcpy(&(this->msg[4]), &_inputStringLength, sizeof(_inputStringLength));

    for (int i = 0; i < _inputStringLength; i++)
        this->msg[8 + i] = _inputString[i];

    memcpy(&(this->msg[this->inputStringLength + 8]), &_userIndex, sizeof(_userIndex));
    return this->msg;
}

void S2C_CHATECO_NTY::Deserialize(char* _inputString)
{
    memset(this->msg, 0, ECO_NTY_BUF_SIZE); // 기존 멤버 변수 초기화
    memcpy(&this->totalBufferSize, &(_inputString[0]), sizeof(int));
    memcpy(&this->inputStringLength, &(_inputString[4]), sizeof(int));

    for (int j = 0; j < inputStringLength; j++)
        this->msg[j] = _inputString[j + 8];

    memcpy(&this->userIndex, &(_inputString[this->inputStringLength + 8]), sizeof(this->userIndex));
}

int S2C_CHATECO_NTY::GetTotalBufferSize()
{
    return this->totalBufferSize;
}

int S2C_CHATECO_NTY::GetInputStringLength()
{
    return this->inputStringLength;
}

char* S2C_CHATECO_NTY::GetMsg()
{
    return this->msg;
}

int S2C_CHATECO_NTY::GetUserIndex()
{
    return this->userIndex;
}