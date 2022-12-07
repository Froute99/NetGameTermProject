#include "stdafx.h"
#include "Client.h"

int PacketSizeHelper(char packetType)
{
	int packetSize;
	switch (packetType)
	{
	case PACKET::PlayerInfo:
	{
		packetSize = sizeof(PlayerInfoPacket);
		break;
	}
	case PACKET::PlayerStatus:
	{
		packetSize = sizeof(PlayerStatusPacket);
		break;
	}
	case PACKET::ItemInfo:
		packetSize = sizeof(ItemInfoPacket);
		break;
	case PACKET::MissileInfo:
	{
		packetSize = sizeof(MissileInfoPacket);
		break;
	}
	default:
		packetSize = -1;
		break;
	}
	return packetSize;
}

void PacketProcessHelper(char packetType, char* fillTarget, Client* client)
{
	switch (packetType)
	{
	case PACKET::PlayerInfo:
	{
		PlayerInfoPacket piPacket;
		memcpy(&piPacket, fillTarget, sizeof(PlayerInfoPacket));
		client->playerData[piPacket.playerNumber] = piPacket;
		break;
	}
	case PACKET::PlayerStatus:
	{
		PlayerStatusPacket psPacket;
		memcpy(&psPacket, fillTarget, sizeof(PlayerStatusPacket));
		client->playerStatus[psPacket.playerNumber] = psPacket;
		break;
	}
	case PACKET::ItemInfo:

		ItemInfoPacket itPacket;
		memcpy(&itPacket, fillTarget, sizeof(ItemInfoPacket));
		for (int i = 0; i < 10; i++)
		{
			if (!client->itemPacket[i].active)
			{
				client->itemPacket[i] = itPacket;
				break;
			}
		}
		break;
	case PACKET::MissileInfo:
	{
		MissileInfoPacket miPacket;
		memcpy(&miPacket, fillTarget, sizeof(MissileInfoPacket));
		for (int i = 0; i < 32; ++i)
		{
			if (!client->missilePacket[i].active)
			{
				client->missilePacket[i] = miPacket;
				break;
			}
			// some where need a garbage collector which clean up missile deactivated
		}
		break;
	}
	default:
		break;
	}
}



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

	if (connect(*sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		err_quit("socket()");

	if (recv(*sock, (char*)&PlayerNum, sizeof(int), MSG_WAITALL) == SOCKET_ERROR)
	{
		err_quit("socket()");
	}
	playerData[PlayerNum].playerNumber = PlayerNum;


	CreateThread(NULL, 0, ReceiveFromServer, this, 0, NULL);


	DWORD optval = 1;
	setsockopt(*sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval));
}

void Client::KeyDownHandler(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'W': sendKey |= option0;	break;      //0000 0001 
		case 'S': sendKey |= option1;	break;      //0000 0010
		case 'A': sendKey |= option2;	break;      //0000 0100
		case 'D': sendKey |= option3;	break;      //0000 1000
		case 'Q': sendKey |= option4;	break;      //0001 0000
		case 'E': sendKey |= option5;	break;      //0010 0000
		case ' ': sendKey |= option6;	break;
		}
	}
}

void Client::KeyUpHandler(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case 'W': sendKey &= (~option0);	break;
		case 'S': sendKey &= (~option1);	break;
		case 'A': sendKey &= (~option2);	break;
		case 'D': sendKey &= (~option3);	break;
		case 'Q': sendKey &= (~option4);	break;
		case 'E': sendKey &= (~option5);	break;
		case ' ': sendKey &= (~option6);	break;
		}
	}
}

void Client::SendtoServer()
{
	PlayerKeyPacket cs_key;
	cs_key.playerKeyInput = sendKey;
	cs_key.mousePosition = deltaMouse;
	if (send(*sock, (char*)&cs_key, sizeof(PlayerKeyPacket), 0) == SOCKET_ERROR)
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

	const int bufSize = 512;
	char buf[bufSize]{};
	while (true)
	{
		ResetEvent(client->ReceiveDone);

		if (recv(*sock, (char*)&buf, bufSize, MSG_WAITALL) == SOCKET_ERROR)
			err_quit("recv()");

		int restBufSize = bufSize;
		int bufOffset = 0;

		// process remain packet
		if (client->remainSize > 0)
		{
			memcpy(&client->remain[client->remainOffset], buf, client->remainSize);
			restBufSize -= client->remainSize;
			PacketProcessHelper(client->remain[0], client->remain, client);
			bufOffset += client->remainSize;

			// reset remain
			memset(client->remain, 0, 512);
			client->remainOffset = 0;
			client->remainSize = 0;
		}

		while (restBufSize > 0)
		{
			char packetType = buf[bufOffset];
			int packetSize = PacketSizeHelper(packetType);

			// save remain packet
			if (restBufSize < packetSize)
			{
				client->remainOffset = restBufSize;
				client->remainSize = packetSize - client->remainOffset;
				memcpy(&client->remain, buf + bufOffset, restBufSize);
				restBufSize -= client->remainSize;
				break;
			}

			// Packet process
			PacketProcessHelper(packetType, buf + bufOffset, client);
			restBufSize -= packetSize;
			bufOffset += packetSize;
		}

		SetEvent(client->ReceiveDone);
	}
}

