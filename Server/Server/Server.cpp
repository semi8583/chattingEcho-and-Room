#define FD_SETSIZE 1028  // 유저 1027명까지 접속 허용
#include <iostream>
#include <map>
#include <cstdlib>
#include <WinSock2.h> // 윈도우 C++환경에서 소켓 프로그래밍을 하기 위해 헤더 포함
#include <windows.h> // Win32 API 함수를 사용하기 위해
#pragma comment(lib, "ws2_32.lib") //#include <WinSock2.h> 사용하기 위한  ws2_32.lib 추가, 이게 있어야 윈도우에 소켓을 사용 가능
using namespace std;
CONST int MAX_LISTENING_QUEUE = 5;
#define BUF_SIZE 512
#include "C2S_CHATECHO_REQ.h"
#include "S2C_CHATECHO_ACK.h"
#include "S2C_CHATECHO_NTY.h"

#include "C2S_ROOM_ENTER_REQ.h"
#include "S2C_ROOM_ENTER_ACK.h"
#include "S2C_ROOM_ENTER_NTY.h"

#include "C2S_PID_REQ.h"

#include <fstream>
#include <string>
#include <ctime>
#include "logger.h"

struct User
{
	int userIndex;
	int bufferSize; // 총 버퍼 길이
	int inputLegnth; // buffer에 들어온 길이;
	char buffer[1024] = { 0, };
	int roomNo = 0;
	bool initialRecv = false;
};

string TimeResult()
{
	time_t timer = time(NULL);// 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
	struct tm t;
	localtime_s(&t,&timer); // 포맷팅을 위해 구조체에 넣기
	cout << (t.tm_year + 1900) << "년 " << t.tm_mon + 1 << "월 " << t.tm_mday << "일 " << t.tm_hour << "시 " << t.tm_min << "분 " << t.tm_sec << "초 ";
	return " ";
}

int main(void)
{
	ofstream file;
	file.open("C:\\Users\\secrettown\\source\\repos\\Server\\Server\\log.txt", ios_base::out | ios_base::app); // 파일 경로(c:\\log.txt)
	//cout.rdbuf(file.rdbuf()); // 표준 출력 방향을 파일로 전환
	ostreamFork osf(file, cout);


	int userNum = 1; // 유저 개수
	map<UINT_PTR, User> userMap;

	CHAR port[10] = { 0, };// = "3587";
	char c;
	ifstream fin("C:\\Users\\secrettown\\source\\repos\\Server\\Server\\port.txt");
	if (fin.fail())
	{
		osf << "포트 파일이 없습니다 " << endl;
		return 0;
	}
	int i = 0;
	while (fin.get(c))
	{
		port[i++] = c;
	}
	fin.close(); // 열었던 파일을 닫는다. 
	//osf << "Enter PORT number (3587) :";
	//cin >> port;

	C2S_CHATECHO_REQ c2sEchoReq;
	S2C_CHATECHO_ACK s2cEchoAck;
	S2C_CHATECHO_NTY s2cEchoNty;

	C2S_ROOM_ENTER_REQ c2sRoomReq;
	S2C_ROOM_ENTER_ACK s2cRoomAck;
	S2C_ROOM_ENTER_NTY s2cRoomNty;

	C2S_PID_REQ c2sPidReq;

	while (1)
	{
		WSADATA wsaData; //WSADATA 구조체 변수 객체 선언
		SOCKET hServerSocket; // 소켓 생성하는 함수
		SOCKET hClientSocket;
		SOCKADDR_IN serverAddr; // SOCKADDR_IN 구조체는 IPv4 주소를 저장하는 구조체
		SOCKADDR_IN clientAddr;

		INT iClientAddrSize; // 주소 정보 길이

		int CurrentUserPid = 0;


		///////////////////////////////
		//  select 함수사용 변수
		TIMEVAL timeout;
		fd_set fd_reads, cpset_reads;
		unsigned int fd_selchang;
		//////////////////////////////////

		//WS2_32.dll을 불러와 메모리에 적재하는 함수
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // WSAStartup함수는 wsaData를 초기화 하는 함수
		//MAKEWORD: 윈도우 소켓 2.2버젼을 사용
			osf << TimeResult() << " 포트 번호: " << port << ", WSAStartup() 함수 에러" << endl; // 문자열 출력과 함께 개행 출력

		hServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);//IPv4 인터넷 프로토콜 체계에서 동작하는 연결지향형 TCP 소켓을 생성하여 반환
		//서버 소켓 만듬  TCP 프로토콜의 소켓
		//서버 소켓: 클라이언트에서 접속을 요청할 경우 받아줄 소켓이 필요 그 소켓이 서버 소켓
		//PF_INET 프로토콜 체계
		// SOCK_STREAM: 연결 지향형 TCP 소켓 (소켓과 소켓이 계속 연결되어 있는 상태 유지, 연결지향형 소켓이란 소켓과 소켓의 연결이 1 vs 1)
		if (hServerSocket == INVALID_SOCKET) // 소켓 생성에 실패할 경우 INVALID_SOCKET 반환
			osf << TimeResult() << " 포트 번호: " << port << ", 서버 소켓 핸들 에러" << endl;
		//////////////////////////////
		//  fd_set 초기화및 할당
		FD_ZERO(&fd_reads);
		FD_SET(hServerSocket, &fd_reads);
		/////////////////////////////////////////
		//
		//  서버주소할당 구조체
		ZeroMemory(&serverAddr, sizeof(serverAddr));//메모리를 0으로 채움
		serverAddr.sin_family = AF_INET; //sockaddr_in 구조체 객체 serverAddr에 변수 값 할당 / AF_INET 주소체계 표현
		serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//주소자동할당,  htonl: 빅엔디안과 리를엔디안이 다를경우
		serverAddr.sin_port = htons(atoi(port));

		//bind란 소켓에 주소를 할당
		if (bind(hServerSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) // 서버컴퓨터의 ip값이랑 포트값을 할당
		//전화기로 비유를 하면 전화를 걸 번호가 필요한데 네트워크에서는 ip와 포트 값이 필요
			osf << TimeResult() << " 포트 번호: " << port << ", 바인드 함수가 정상적으로 작동되지 못하였습니다." << endl;;
		// bind 리턴값으로 성공 시 0, 실패시 SOCKET_ERROR 출력
		osf << TimeResult() << " 포트 번호: " << port << ", [bind] from server" << endl;

		if (listen(hServerSocket, MAX_LISTENING_QUEUE) == SOCKET_ERROR) // listen함수로 연결 요청 대기 상태(클라이언트에 연결을 요청)
		//MAX_LISTENING_QUEUE 요청 가능한 최대 접속 승인 수
			osf << TimeResult() << " 포트 번호: " << port << ", 리슨 함수가 제대로 작동되지 못하였습니다." << endl;;
		osf << TimeResult() << " 포트 번호: " << port << ", [listen] from server" << endl;

		while (1)//서버에서 연결 끊기지 않고 계속적으로 클라이언트 측에서 요청 받을 수 있게
		{
			cpset_reads = fd_reads; //체크상태을 복사저장한다
			timeout.tv_sec = 5;     //타임벨 5초
			timeout.tv_usec = 5000; //타임벨 0.005초
							 //select(갯수,*수신체크,*송신체크,*이벤트체크,타임벨)

			if ((fd_selchang = select(0, &cpset_reads, 0, 0, &timeout)) == SOCKET_ERROR)
				break;

			if (fd_selchang == 0)
				continue;
			// 윈도우는  fd_set 구조체을 사용하며 그안에 갯수정보가 포함된다.
			//루프문으로  수신체크변화 상태을 감시한다.   

			int fd_count = fd_reads.fd_count;
			for (int j = 0; j < fd_count; j++)
			{
				//변화된 배열을 복사된체크상태와 비교하여변화가 비교
				if (FD_ISSET(fd_reads.fd_array[j], &cpset_reads))
				{
					//배열과 소켓핸들과 비교하여 참이면 접속 수락 하고
					//접속클라이언트핸들을 fd_set에 추가한다.
					if (fd_reads.fd_array[j] == hServerSocket)     // connection request!
					{
						memset(userMap[hClientSocket].buffer, 0, BUF_SIZE);// 유저가 새로 들어오면 버퍼 초기화

						iClientAddrSize = sizeof(clientAddr);
						hClientSocket = accept(hServerSocket, (SOCKADDR*)&clientAddr, &iClientAddrSize);// 연결 요청 수락
						osf << TimeResult() << " 포트 번호: " << port << ", [accept] from server" << endl;
						// 이 함수는 동기화된 방식으로 동작 => 동기화된 방식이란 요청을 마무리 하기 전까지 계속 대기 상태에 놓임
						// 요청이 들어오기 전까지 이 함수는 안빠져나옴

						if (hClientSocket == INVALID_SOCKET)
							osf << TimeResult() << " 포트 번호: " << port << ", 클라이언트 소켓이 잘못되었습니다.!!" << endl;

						FD_SET(hClientSocket, &fd_reads);
						userMap[hClientSocket].userIndex = userNum++;
						userMap[hClientSocket].roomNo = 0;

						osf << TimeResult() << " 포트 번호: " << port << " 유저 " << userMap[hClientSocket].userIndex << " 번째 Client" << endl;
						
						int code = 3;
						c2sPidReq.Serialize(12 , code, userMap[hClientSocket].userIndex);
						char tmpBuffer[BUF_SIZE] = { 0, };
						memset(tmpBuffer, 0, BUF_SIZE);
						for (int i = 0; i < 12; i++)
							tmpBuffer[i] = c2sPidReq.GetMsg()[i];
						c2sPidReq.Deserialize(tmpBuffer);
						send(hClientSocket, c2sPidReq.GetMsg(), c2sPidReq.GetSize(), NULL);
					}
					//서버소켓의 변화가 아니라면 메세지을 수신한다.
					//메세지 수신후 확인하여
					//클라이언트 fd_set의 상태체크을 해제하고 소켓을 닫는다(??).
					else    // read message!
					{
						char tmpBuffer[BUF_SIZE] = { 0, }; // 통신이 2번 이상 있을 때 문자열을 연결 해줘야 하는데 recv에서 잠시 문자열 받아주는 변수


						int iMessageLen = recv(fd_reads.fd_array[j], tmpBuffer, BUF_SIZE, 0);// 클라이언트 소켓에서 입력된 숫자를 받음
						if (iMessageLen == -1 || iMessageLen == 0) // recv 실패 시
						{
							send(cpset_reads.fd_array[j], tmpBuffer, BUF_SIZE, NULL);
							FD_CLR(fd_reads.fd_array[j], &fd_reads); // FD_SET 함수를 사용해서 추가했던 FD값을 제거할 때

							closesocket(cpset_reads.fd_array[j]);//소켓 종료

							osf << TimeResult() << " 포트 번호: " << port << ", closed client : " << userMap[cpset_reads.fd_array[j]].userIndex << " 번째 Client" << endl;
						}
						else if (strlen(tmpBuffer) == 0) // recv로 받은 문자열이 없을 때 
						{
							break;
						}
						else
						{
							c2sPidReq.Deserialize(tmpBuffer);
							if (c2sPidReq.GetCode() == 2) // 채팅 방 번호 선택
							{
								c2sRoomReq.Deserialize(tmpBuffer); // c2sreq로 변경 s2cRoomNty.GetMsg() 이거 자체를 넣어버리면 char*가 deserialize에서 초기화 됨 

								for (int k = 0; k < fd_reads.fd_count; k++)// -1(0) defualt 방
								{
									if (userMap[fd_reads.fd_array[k]].userIndex == c2sRoomReq.GetUserIdx())
										CurrentUserPid = fd_reads.fd_array[k];
								}

								userMap[CurrentUserPid].roomNo = c2sRoomReq.GetRoomNo();

								int roomNo;
								c2sRoomReq.GetRoomNo() == 0 ? roomNo = -1 : roomNo = c2sRoomReq.GetRoomNo();

								s2cRoomAck.Serialize(c2sRoomReq.GetSize(), c2sRoomReq.GetCode(), c2sRoomReq.GetRoomNo(), 1);
								userMap[CurrentUserPid].bufferSize = c2sRoomReq.GetSize();
								userMap[CurrentUserPid].userIndex = c2sRoomReq.GetUserIdx();
								for(int i = 0; i < userMap[CurrentUserPid].bufferSize; i++)
									userMap[CurrentUserPid].buffer[i] = c2sRoomReq.GetMsg()[i];

								osf << "\n" << TimeResult() << " 포트 번호: " << port << ", [recv] msg received. 총 버퍼 사이즈 : \"" << userMap[CurrentUserPid].bufferSize << "\" , Code(1: 채팅 에코, 2: 채팅 룸 입장): \"" << c2sRoomReq.GetCode() << "\" , Result(1: 방 입장 성공, 0: 방 입장 실패): \"" << 1 << "\" , RoomNo: \"" << roomNo << "\" from server \"" << userMap[CurrentUserPid].userIndex << "\" 번째 Client" << endl;// 받은 숫자를 콘솔 창에 출력

								for (int k = 0; k < fd_reads.fd_count; k++)// -1(0) defualt 방
								{
									if (userMap[fd_reads.fd_array[k]].roomNo == userMap[CurrentUserPid].roomNo )
										send(fd_reads.fd_array[k], userMap[CurrentUserPid].buffer, 1024, NULL); //tmpBuffer -> sc2NTY
								}

								char tmpBuffer[BUF_SIZE] = { 0, };
								memset(tmpBuffer, 0, BUF_SIZE);
								for (int i = 0; i < c2sRoomReq.GetSize(); i++)
									tmpBuffer[i] = c2sRoomReq.GetMsg()[i];
								s2cRoomNty.Deserialize(tmpBuffer);
								osf << TimeResult() << " 포트 번호: " << port << ", [send] msg received. 총 버퍼 사이즈 : \"" << s2cRoomNty.GetSize() << "\" , Code(1: 채팅 에코, 2: 채팅 룸 입장): \"" << s2cRoomNty.GetCode() << "\" , Result(1: 방 입장 성공, 0: 방 입장 실패): \"" << 1 << "\" , RoomNo: \"" << roomNo << "\" from server \"" << s2cRoomNty.GetUserIdx() << "\" 번째 Client" << endl;// 받은 숫자를 콘솔 창에 출력							
							}
							else //  채팅 에코 프로그램
							{
								c2sEchoReq.Deserialize(tmpBuffer); // c2sreq로 변경 s2cRoomNty.GetMsg() 이거 자체를 넣어버리면 char*가 deserialize에서 초기화 됨 

								for (int k = 0; k < fd_reads.fd_count; k++)// -1(0) defualt 방
								{
									if (userMap[fd_reads.fd_array[k]].userIndex == c2sEchoReq.GetUserIdx())
										CurrentUserPid = fd_reads.fd_array[k];
								}

								if (userMap[CurrentUserPid].initialRecv == false) // 처음 recv 받으면
								{
									int size = 0;
									for (size = 0; size < BUF_SIZE; size++)
									{
										userMap[CurrentUserPid].buffer[size] = tmpBuffer[size];
										if (size >= c2sEchoReq.GetSize() && tmpBuffer[size] == 0)
											break;
									}
									userMap[CurrentUserPid].bufferSize = c2sEchoReq.GetSize();
									userMap[CurrentUserPid].inputLegnth = size;
									userMap[CurrentUserPid].userIndex = c2sEchoReq.GetUserIdx();
									userMap[CurrentUserPid].initialRecv = true;

									if (userMap[CurrentUserPid].inputLegnth != userMap[CurrentUserPid].bufferSize) // 여기서 inputLegnth는 총 버퍼 사이즈를 말한다 같지 않으면 recv가 또 있다는 말이다
										break;
								}
								else // 2번이상 recv 받을 때  
								{
									int size;
									for (size = 0; size < BUF_SIZE; size++)
									{
										userMap[CurrentUserPid].buffer[userMap[CurrentUserPid].inputLegnth + size] = tmpBuffer[size]; // 기존에 있던 버퍼에 연결
									}
									userMap[CurrentUserPid].inputLegnth += size;
									if (userMap[CurrentUserPid].inputLegnth != userMap[CurrentUserPid].bufferSize)// 여기서 inputLegnth는 총 버퍼 사이즈를 말한다 같지 않으면 recv가 또 있다는 말이다
										break;
								}
								c2sEchoReq.Deserialize(userMap[CurrentUserPid].buffer);
								userMap[CurrentUserPid].bufferSize = c2sEchoReq.GetSize();
								userMap[CurrentUserPid].inputLegnth = c2sEchoReq.GetStringLength();
								userMap[CurrentUserPid].userIndex = c2sEchoReq.GetUserIdx();
								osf << TimeResult() << " 포트 번호: " << port << ", [recv] msg received. 전체 사이즈 : \"" << userMap[CurrentUserPid].bufferSize << "\" , Code(1: 에코 메시지 전송, 2: 채팅 룸 입장): \"" << c2sEchoReq.GetCode()  << "\" , 문자열 길이: \"" << userMap[CurrentUserPid].inputLegnth << "\" , 문자열: \"" << c2sEchoReq.GetMsg() << "\" from server \"" << userMap[CurrentUserPid].userIndex << "\" 번째 Client" << endl;// 받은 숫자를 콘솔 창에 출력

								for (int k = 0; k < fd_reads.fd_count; k++)
								{
									if (userMap[fd_reads.fd_array[k]].roomNo == userMap[CurrentUserPid].roomNo)
										send(fd_reads.fd_array[k], userMap[CurrentUserPid].buffer, 1024, NULL); // 다시 클라이언트에 받은 숫자를 보냄
								}

								osf << TimeResult() << " 포트 번호: " << port << ", [send] msg received. 전체 사이즈 : \"" << userMap[CurrentUserPid].bufferSize << "\" , Code(1: 에코 메시지 전송, 2: 채팅 룸 입장): \"" << c2sEchoReq.GetCode() << "\" , 문자열 길이: \"" << userMap[CurrentUserPid].inputLegnth << "\" , 문자열: \"" << c2sEchoReq.GetMsg() << "\" from server \"" << userMap[CurrentUserPid].userIndex << "\" 번째 Client" << endl;// 받은 숫자를 콘솔 창에 출력
							}
						}
					}
				}
			}
		}
	}
	return 0;
}