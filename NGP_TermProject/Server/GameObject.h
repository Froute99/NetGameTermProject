#pragma once

#include "stdafx.h"
#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20
class GameObject {
public:

	XMFLOAT4X4 m_xmf4x4World;
	BoundingOrientedBox m_xmOOBB;
	XMFLOAT3 m_xmf3MovingDirection;
	XMFLOAT3 m_xmf3RotationAxis;

	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;

	float           			m_fPitch;
	float           			m_fYaw;
	float           			m_fRoll;

	bool m_bActive;//active
	float m_fSpeed;//speed
	int m_nObjects{};

	int Player_id;

	void Move();
	void Rotate(float Pitch, float Yaw, float Roll);
	void SetPosition(float x, float y, float z);
	void SetActive(bool active);
	bool GetActive();
	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation)
	{
		m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation);
	}
	BoundingOrientedBox GetBoundingBox() { return m_xmOOBB; }

};

class CMissileObject :public GameObject
{
	char m_cPlayerNumber;
};
class CPlayer : public GameObject
{
public:
	float m_fFriction;
	int m_nHp;
	CMissileObject m_pMissiles[8];
	void Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(DWORD Direction, float Distance, bool updateVelocity);
};

class Item : public GameObject
{

};