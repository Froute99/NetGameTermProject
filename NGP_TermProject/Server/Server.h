#pragma once

#include "Socket.h"
#include "SCPacket.h"
#include "GameObject.h"

#include <array>
#include <chrono>
#include <queue>

#define SERVERPORT		9000
#define BUFSIZE			512

#define MAX_CLIENT_NUM		4
#define MAX_ITEM_NUM		10

#define RESPAWN_TIME		5.f

#define MAX_BOUNDARY_X		1000
#define MIN_BOUNDARY_X		0
#define MAX_BOUNDARY_Y		1000
#define MIN_BOUNDARY_Y		300
#define MAX_BOUNDARY_Z		1000
#define MIN_BOUNDARY_Z		0

DWORD WINAPI ReceiveFromClient(LPVOID arg);
DWORD WINAPI AcceptClient(LPVOID arg);

class Client;
class CPlayer;
class CItemObject;

class Clock {
public:
	using time = std::chrono::high_resolution_clock;
	using second = std::chrono::duration<float>;

	Clock()	: timePassed(0.f), timeStamp(time::now()) {}
	void Record() { timeStamp = time::now(); }
	float GetTimePassedFromLastUpdate() { return second(time::now() - timeStamp).count(); }
private:
	float timePassed;
	time::time_point timeStamp;
};

class Server {
public:
	Server();
	~Server();

	void OpenListenSocket();

	void SendAllClient();

	// 클라 충돌 및 움직임 송수신
	void Update();
	void CheckCollision();
	void SpawnItem();

	SOCKET* GetSocket() { return &listenSock; }

	Clock timer;
	const float itemRespawnTime = 8.f;
	float itemSpawnTime = 0.f;
	float elapsedTime = 0.f;

	std::array<Client*, MAX_CLIENT_NUM> clients;

	int connectedClients = 0;
	HANDLE updateDone;

	CItemObject* m_ItemObject[MAX_ITEM_NUM];
	std::queue<GameObject*> trashCan;
private:
	SOCKET listenSock;
};


class Client {
public:
	Client();
	~Client();
	SOCKET sock;

	void SetPlayerNumber(int playerNumber) { m_playerNumber = (char)playerNumber; }
	int GetPlayerNumber() { return m_playerNumber; }

	void Connected() { m_connected = true; shouldDisconnected = false; }
	bool IsConnected() { return m_connected; }

	bool ShouldDisconnected() { return shouldDisconnected; }
	void Disconnect() { m_connected = false; shouldDisconnected = false; }

	void Reset();

	CPlayer* m_player = nullptr;

	float deadTime = 0.f;
private:
	int m_playerNumber = -1;	// maybe client class can have playerID inside

	bool m_connected = false;
	bool shouldDisconnected = false;
};

