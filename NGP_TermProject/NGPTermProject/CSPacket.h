#pragma once
#include"stdafx.h"
const char CL_PlayerInfo = 0;
const char CL_PlayerStatus = 1;


#pragma pack(1)
struct PlayerInfoPacket {
	char packetType;
	int playerNumber = -1;
	XMFLOAT3 movement;
	XMFLOAT3 rotation;
};

struct PlayerStatusPacket {
	char packetType;
	int playerNumber;
	unsigned char activatedMissiles;
	int playerHP;
};

struct ItemInfoPacket {
	char packetType;
	float x, y, z;
	bool active;
};

struct MissileInfoPacket {
	char packetType;
	int playerNumber;
	XMFLOAT3 movement;
	XMFLOAT3 rotation;
};
#pragma pack()

