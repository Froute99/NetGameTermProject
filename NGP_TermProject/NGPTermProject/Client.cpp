#include "stdafx.h"
#include "Client.h"


Client::Client()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	sock = new SOCKET();
}

Client::~Client()
{
	closesocket(*sock);
	WSACleanup();
}

void Client::ConnectServer()
{
	if ((*sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		err_quit("socket()");

	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(SERVERPORT);

	std::cout << "Socket initalize successful\n";

	if (connect(*sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)	//err_quit("socket()");
		std::cout << "Connection to server not established\n";
	else
		std::cout << "Connection established successful\n";

	recv(*sock, (char*)&PlayerNum, sizeof(int), 0);

	playerData[PlayerNum].playerNumber = PlayerNum;


	//DWORD optval = 1;
	//setsockopt(*sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval));
}

void Client::SendtoServer(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		//cout << "메세지 보내는중" << endl;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W': sendKey |= option0;	break;	//0000 0001 
		case 'S': sendKey |= option1;	break;	//0000 0010
		case 'A': sendKey |= option2;	break;	//0000 0100
		case 'D': sendKey |= option3;	break;	//0000 1000
		case 'Q': sendKey |= option4;	break;	//0001 0000
		case 'R': sendKey |= option5;	break;	//0010 0000
		}
	}
	char buf[1];
	buf[0] = sendKey;
	sendKey = NULL;


	int sentBytes = send(*sock, buf, 1, 0);
	if (sentBytes == SOCKET_ERROR)
	{
		err_display("send()");
		return;
	}
	//buf[0] = (char)&ptCursorPos;
}

DWORD WINAPI ReceiveFromServer(LPVOID arg)
{
	Client* client = (Client*)arg;
	SOCKET* sock = client->GetClientsock();
	while (true)
	{
		PlayerInfoPacket piPacket;
		int retval = recv(*sock, (char*)&piPacket, sizeof(PlayerInfoPacket), 0);
		client->playerData[piPacket.playerNumber] = piPacket;   //->Player and otherPlayer render ->goto Scene.cpp render() and Player.cpp render()
	}
}

