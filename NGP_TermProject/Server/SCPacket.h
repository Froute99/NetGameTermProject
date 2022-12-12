#pragma once
#pragma warning(disable : 26495)

#include "stdafx.h"

const char SC_PlayerInfo = 0;
//const char SC_PlayerKey = 1;
const char SC_ItemInfo = 1;
const char SC_MissileInfo = 2;


struct FPoint {
	float x;
	float y;
}; 


#pragma pack(1)
struct PlayerInfoPacket {
	char packetType;
	int playerNumber = -1;
	int playerHP;
	XMFLOAT3 movement;
	XMFLOAT3 rotation;
	bool playerActive;
};
#pragma pack()

#pragma pack(1)
struct PlayerKeyPacket {
	unsigned char playerKeyInput;
	FPoint deltaMouse;
};
#pragma pack()

#pragma pack(1)
struct ItemInfoPacket {
	char packetType;
	int itemNum;
	XMFLOAT3 position;
	bool active;
};
#pragma pack()

#pragma pack(1)
struct MissileInfoPacket {
	char packetType;
	int playerNumber;
	int missileNumber;
	bool active = false;
	XMFLOAT3 movement;
	XMFLOAT3 rotation;
};
#pragma pack()

