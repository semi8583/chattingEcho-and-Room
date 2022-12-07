#include <iostream>
#include <WinSock2.h>// 윈도우 C++환경에서 소켓 프로그래밍을 하기 위해 헤더 포함
#include <windows.h>// Win32 API 함수를 사용하기 위해
#include <thread>
#include <charconv>
#include <WS2tcpip.h>
#include "C2S_CHATECO_REQ.h"
#include "S2C_CHATECO_ACK.h"
#include "S2C_CHATECO_NTY.h"

#include "C2S_ROOM_ENTER_REQ.h"
#include "S2C_ROOM_ENTER_ACK.h"
#include "S2C_ROOM_ENTER_NTY.h"

#pragma comment(lib, "ws2_32.lib")//#include <WinSock2.h> 사용하기 위한  ws2_32.lib 추가, 이게 있어야 윈도우에 소켓을 사용 가능
using namespace std;

static CHAR IP[] = "127.0.0.1";
static CHAR Port[] = "3587";
#define ROOM_NTY_BUF_SIZE 1024

SOCKET hSocket; // 소켓 생성하는 함수
bool ThreadStop = true;
C2S_CHATECO_REQ c2sEcoReq;
S2C_CHATECO_ACK s2cEcoAck;
S2C_CHATECO_NTY s2cEcoNty;

C2S_ROOM_ENTER_REQ c2sRoomReq;
S2C_ROOM_ENTER_ACK s2cRoomAck;
S2C_ROOM_ENTER_NTY s2cRoomNty;

int MenuNum;


void Char_Recv()//클라이언트에서 문자열 입력을 받는 도중에 서버에서 문자열을 받으면 스레드를 이용해서 밑에 함수 실행함
{
	while (ThreadStop) // 소켓이 돌고 있으므로 소켓 먼저 종료 시키고 스레드 종료 시키면 정상 종료
	{
		char tmpBuffer[ROOM_NTY_BUF_SIZE] = { 0, };
		if (recv(hSocket, tmpBuffer, ROOM_NTY_BUF_SIZE, 0)) //recv(소켓, 수신 정보를 담을 배열주소, 그 배열의 크기, flag)
		{ //  flags의 값이 0이면 일반 데이터를 수신
		  //대상 소켓으로부터 보내온 정보를 받아주는 역할
		  //보내준 데이터가 없다면 여기에서 받을 때까지 계속 대기 상태에 있음
			int totalBufferSize;

			memcpy(&totalBufferSize, &tmpBuffer, sizeof(totalBufferSize));
			if (tmpBuffer[0] == '\0')
				break;

			if (totalBufferSize == 10) // 채팅 방 입장
			{
				s2cRoomNty.Deserialize(tmpBuffer);
				cout << "\n[recv] msg received. 총 버퍼 사이즈 : \"" << s2cRoomNty.GetTotalBufferSize() << "\", Code: \"" << s2cRoomNty.GetCode() << "\" , Result: \"" << s2cRoomNty.GetResult() << "\" from server \"" << s2cRoomNty.GetUserIndex() << "\" 번째 Client" << endl;// 받은 숫자를 콘솔 창에 출력
			} 
			else // 채팅 에코 프로그램 
			{
				s2cEcoNty.Deserialize(tmpBuffer);
				cout << "\n[recv] msg received. 전체사이즈 : \"" << s2cEcoNty.GetTotalBufferSize() << "\", 문자열 길이: \"" << s2cEcoNty.GetInputStringLength() << "\" , 문자열: \"" << s2cEcoNty.GetMsg() << "\" from server \"" << s2cEcoNty.GetUserIndex() << "\" 번째 Client" << endl;// 받은 숫자를 콘솔 창에 출력
			}

			if (MenuNum == 1) // 채팅 에코 메시지
				cout << "\n문자를 입력해주세요: (-1 입력 시 종료) ";
			else // 채팅 룸 입장
				cout << "\n입장할 채팅 방 번호를 입력 하세요!: (입력 안할시 default: -1, -2 입력시 종료) ";
		}
		tmpBuffer[0] = '\0';// 버퍼에 문자열이 남으면 통신이 또 실행 할수도 있어서 문자열 초기화
	}
}

void MenuSelection()
{
	cout << "메뉴를 선택해 주세요!" << endl;
	cout << "1. 채팅 에코 메시지 전송" << endl;
	cout << "2. 채팅 룸 입장" << endl;
	cout << "0. 프로그램 종료" << endl;
}

void CommonFunc()
{
	WSADATA wsaData;//WSADATA 구조체 변수 객체 선언
	SOCKADDR_IN serverAddr;// SOCKADDR_IN 구조체는 IPv4 주소를 저장하는 구조체

	//WS2_32.dll을 불러와 메모리에 적재하는 함수
	// WSAStartup함수는 wsaData를 초기화 하는 함수
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{//MAKEWORD: 윈도우 소켓 2.2버젼을 사용
		cout << "WSAStartup() 함수 에러" << endl;// 문자열 출력과 함께 개행 출력
		exit(1);// WSAStartup이라는 함수는 성공시 0, 실패 시 SOCKET_ERROR를 리턴
	}

	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//AF_INET -> IPv4 socket, AF_INET6 -> IPv6 socket

	//서버 소켓 만듬  TCP 프로토콜의 소켓
	//서버 소켓: 클라이언트에서 접속을 요청할 경우 받아줄 소켓이 필요 그 소켓이 서버 소켓
	//PF_INET 프로토콜 체계
	// SOCK_STREAM: 연결 지향형 TCP 소켓 (소켓과 소켓이 계속 연결되어 있는 상태 유지, 연결지향형 소켓이란 소켓과 소켓의 연결이 1 vs 1)
	if (hSocket == INVALID_SOCKET)// 소켓 생성에 실패할 경우 INVALID_SOCKET 반환
	{
		cout << "소켓 핸들 에러" << endl;
		exit(1);
	}

	ZeroMemory(&serverAddr, sizeof(serverAddr));//메모리를 0으로 채움
	serverAddr.sin_family = AF_INET;//sockaddr_in 구조체 객체 serverAddr에 변수 값 할당 / AF_INET 주소체계 표현
	//serverAddr.sin_addr.S_un.S_addr = inet_addr(IP);
	serverAddr.sin_port = htons(atoi(Port));
	serverAddr.sin_family = AF_INET;//sockaddr_in 구조체 객체 serverAddr에 변수 값 할당 / AF_INET 주소체계 표현
	InetPton(AF_INET, IP, &(serverAddr.sin_addr.S_un));

	if (connect(hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{//connect(소켓, 소켓 구성요소 구조체의 주소, 그 구조체의 크기);
		cout << "접속이 끊겼음...." << endl;
		exit(1);
	}// 지정된 소켓에 연결을 설정 (서버에 연결 하기 위해)
	cout << "[connect] from client " << endl;
}

void ChattingEcho()
{
	ThreadStop = true;
	cout << "\n채팅 에코 프로그램  " << endl;
	CommonFunc();

	cout << "\n문자를 입력해주세요: (-1 입력 시 종료) ";
	thread th2(Char_Recv); // 함수를 이용해 스레드 객체 생성
	while (1)
	{
		char inputBuffer[ROOM_NTY_BUF_SIZE] = { 0, };
		memset(inputBuffer, 0, ROOM_NTY_BUF_SIZE);
		cin.getline(inputBuffer, ROOM_NTY_BUF_SIZE, '\n');
		cin.clear();// 입력 버퍼 초기화

		if (inputBuffer[0] == '-' && inputBuffer[1] == '1')    //종료문자 처리
		{
			cout << "클라이언트 종료!" << endl;
			ThreadStop = false;
			break;
		}
		if (inputBuffer != "")
		{
			cout << "[ACK] 문자열 입력 성공!" << endl;
		}
		int totalStringLength = strlen(inputBuffer) + 8;
		int inputStringLegnth = strlen(inputBuffer); // 입력된 문자열 길이
		c2sEcoReq.Serialize(totalStringLength, inputStringLegnth, inputBuffer);
		s2cEcoAck.Serialize(totalStringLength + 2, 1, NULL, inputStringLegnth, inputBuffer);

		char tmpMsg[ROOM_NTY_BUF_SIZE] = { 0, };
		memset(tmpMsg, 0, ROOM_NTY_BUF_SIZE);
		memcpy(&tmpMsg, s2cEcoAck.GetMsg(), totalStringLength + 2);
		s2cEcoAck.Deserialize(tmpMsg);
		s2cEcoAck.SetResult(1); // 문자열 전송 성공

		if (send(hSocket, c2sEcoReq.GetMsg(), ROOM_NTY_BUF_SIZE, 0))// 입력 받은 문자를 서버에 보냄
			cout << "[ACK] [send] " << "총 버퍼 사이즈: \"" << s2cEcoAck.GetTotalBufferSize() << "\" Code: \"" << s2cEcoAck.GetCode() << "\" Result : \"" << s2cEcoAck.GetResult() << "\" , 문자열 길이: \"" << s2cEcoAck.GetInputStringLength() << "\" , 문자열: \"" << s2cEcoAck.GetMsg() << "\" from client 문자열 전송 성공" << endl;
		else
		{
			s2cEcoAck.SetResult(0); // 문자열 전송 실패
			cout << "[ACK] [send] " << "총 버퍼 사이즈: \"" << s2cEcoAck.GetTotalBufferSize() << "\" Code: \"" << s2cEcoAck.GetCode() << "\" Result: \"" << s2cEcoAck.GetResult() << "\" , 문자열 길이: \"" << s2cEcoAck.GetInputStringLength() << "\" , 문자열: \"" << s2cEcoAck.GetMsg() << "\" from client 문자열 전송 실패" << endl;
		}
	}
	s2cEcoAck.SetResult(0); // 클라이언트 종료
	cout << "\n채팅 에코 프로그램  종료" << endl;
	cout << "[ACK] 클라이언트 종료" << endl;
	closesocket(hSocket);// 해당 소켓 닫아준다
	WSACleanup();
	th2.join(); // 해당 쓰레드가 실행을 종료하면 리턴
}

void ChattingRoom()
{
	ThreadStop = true;
	cout << "\n채팅 룸 프로그램  " << endl;
	CommonFunc();
	thread th1(Char_Recv); // 함수를 이용해 스레드 객체 생성
	cout << "\n입장할 채팅 방 번호를 입력 하세요!: (입력 안할시 default: -1, -2 입력시 종료) ";
	while (1)
	{
		char inputBuffer[ROOM_NTY_BUF_SIZE] = { 0, };
		int roomNo;
		cin.getline(inputBuffer, 10);
		if (strlen(inputBuffer) == 0)
			roomNo = -1; // 방 번호 입력 안할시 -1번 방 선택
		else
			roomNo = atoi(inputBuffer); // 1번방 또는 2번 방

		if (roomNo == -2)    //종료문자 처리
		{
			cout << "클라이언트 종료!" << endl;
			ThreadStop = false;
			break;
		}
		else if (roomNo == -1) // -1 번 방 
		{
			c2sRoomReq.SetCode(0);
			cout << "[ACK] -1번방 선택!" << endl;
		}
		else if (roomNo == 1 || roomNo == 2)// 1번방 또는 2번 방
		{
			c2sRoomReq.SetCode(atoi(inputBuffer));
			cout << "[ACK] " << inputBuffer << "번방 선택!" << endl;
		}
		else // 없는 방 번호 일 때 
		{
			c2sRoomReq.SetCode(4);
			cout << "[ACK] 없는 방 입니다. 방을 다시 입력 하세요" << endl;
			cout << "\n입장할 채팅 방 번호를 입력 하세요!: (입력 안할시 default: -1, -2 입력시 종료) ";
			continue;
		}

		s2cRoomAck.Deserialize(c2sRoomReq.Serialize(6, c2sRoomReq.GetCode(), 0)); // 문자열 입력 성공

		s2cRoomAck.SetResult(1); // 채팅 방 입장 성공
		if (send(hSocket, s2cRoomAck.GetMsg(), ROOM_NTY_BUF_SIZE, 0))// 입력 받은 문자를 서버에 보냄
			cout << "[ACK] [send] \"" << "총 버퍼 사이즈: \"" << s2cRoomAck.GetTotalBufferSize() << "\" , code: \"" << s2cRoomAck.GetCode() << "\" , result: \"" << s2cRoomAck.GetResult() << "\" from client 채팅 방 입장 성공" << endl; // 출력할땐 문자열만 출력되게
		else // 서버에 보내는 것을 실패 했을 때
		{
			s2cRoomAck.SetResult(0); // 채팅 방 입장 실패
			cout << "[ACK] [send] \"" << "총 버퍼 사이즈: \"" << s2cRoomAck.GetTotalBufferSize() << "\" , code: \"" << s2cRoomAck.GetCode() << "\" , result: \"" << s2cRoomAck.GetResult() << "\" from client 채팅 방 입장 실패" << endl;
		}
	}
	cout << "\n채팅 룸 프로그램  종료" << endl;
	cout << "[ACK] 클라이언트 종료" << endl;
	closesocket(hSocket);// 해당 소켓 닫아준다
	WSACleanup();
	th1.join(); // 해당 쓰레드가 실행을 종료하면 리턴
}

INT main(int argc, char* argv[])
{
	while (1)
	{
		MenuSelection();
		cin >> MenuNum;
		cin.ignore();// 입력 버퍼 초기화
		if (MenuNum == 1) // 채팅 에코
			ChattingEcho();
		else if (MenuNum == 2) // 채팅룸 입장
			ChattingRoom();
		else if (MenuNum == 0) // 프로그램종료
			break;
		else
			cout << "다시 입력해 주세요!" << endl;
	}
	return 0;
}