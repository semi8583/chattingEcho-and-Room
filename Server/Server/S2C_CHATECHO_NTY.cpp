#include "S2C_CHATECHO_NTY.h"
char* S2C_CHATECHO_NTY::Serialize(int _size, int _code, int _stringLength, int _userIdx, char* _msg)
{
    memset(this->msg, 0, ECHO_NTY_BUF_SIZE); // 기존 멤버 변수 초기화

    memcpy(&(this->msg[0]), &_size, sizeof(_size));
    memcpy(&(this->msg[4]), &_code, sizeof(_code));
    memcpy(&(this->msg[8]), &_stringLength, sizeof(_stringLength));
    memcpy(&(this->msg[12]), &_userIdx, sizeof(_userIdx));

    for (int i = 0; i < _stringLength; i++)
        this->msg[16 + i] = _msg[i];

    return this->msg;
}

void S2C_CHATECHO_NTY::Deserialize(char* _inputString)
{
    memset(this->msg, 0, ECHO_NTY_BUF_SIZE); // 기존 멤버 변수 초기화
    memcpy(&this->size, &(_inputString[0]), sizeof(int));
    memcpy(&this->code, &(_inputString[4]), sizeof(int));
    memcpy(&this->stringLength, &(_inputString[8]), sizeof(int));
    memcpy(&this->userIdx, &(_inputString[12]), sizeof(int));

    for (int j = 0; j < this->stringLength; j++)
        this->msg[j] = _inputString[j + 16];
}

int S2C_CHATECHO_NTY::GetSize()
{
    return this->size;
}

int S2C_CHATECHO_NTY::GetCode()
{
    return this->code;
}

int S2C_CHATECHO_NTY::GetStringLength()
{
    return this->stringLength;
}

char* S2C_CHATECHO_NTY::GetMsg()
{
    return this->msg;
}

int S2C_CHATECHO_NTY::GetUserIdx()
{
    return this->userIdx;
}