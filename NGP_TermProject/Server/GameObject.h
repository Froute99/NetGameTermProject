#pragma once

#include "stdafx.h"
class GameObject {
public:

	XMFLOAT4X4 m_xmf4x4World;
	BoundingOrientedBox m_xmOOBB;
	XMFLOAT3 m_xmf3MovingDirection;
	XMFLOAT3 m_xmf3RotationAxis;

	float m_fSpeed;

	void Move();
	virtual void Rotate(float Pitch, float Yaw, float Roll);
	void SetPosition(float x, float y, float z);

};

class Missile :public GameObject
{
	char m_cPlayerNumber;
	bool m_bActive;
};

class Player : public GameObject
{
public:
	float						m_fFriction;

	float           			m_fPitch;
	float           			m_fYaw;
	float           			m_fRoll;
	int m_nHp;
	Missile m_pMissiles[8];

	void Move(DWORD Direction, float Distance, bool updateVelocity);
	virtual void Rotate(float Pitch, float Yaw, float Roll);
};

class Item : public GameObject
{
	bool m_bActive;
};
