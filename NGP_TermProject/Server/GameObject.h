#pragma once
#pragma warning(disable : 26495)

#include "stdafx.h"

#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20


class GameObject {
public:
	GameObject();
	XMFLOAT4X4 m_xmf4x4World;
	BoundingOrientedBox m_xmOOBB;
	XMFLOAT3 m_xmf3MovingDirection;
	XMFLOAT3 m_xmf3RotationAxis;

	XMFLOAT3					m_xmf3Position;
	XMFLOAT3					m_xmf3Right;
	XMFLOAT3					m_xmf3Up;
	XMFLOAT3					m_xmf3Look;


	void RecalculateLook() { m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33)); }
	void RecalculateRight() { m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13)); }


	float           			m_fPitch = 0.f;
	float           			m_fYaw = 0.f;
	float           			m_fRoll = 0.f;

	float						m_fOldxPos = 0.f;
	float						m_fOldyPos = 0.f;
	float						m_fOldzPos = 0.f;

	float						m_fxPos = 0.f;
	float						m_fyPos = 0.f;
	float						m_fzPos = 0.f;

	float						m_fOldPitch = 0.f;
	float						m_fOldYaw = 0.f;
	float						m_fOldRoll = 0.f;

	bool						m_bActive{ false};//active
	float						m_fSpeed;//speed
	int							m_nObjects{};

	int Player_id;
	int cnt{};

	XMFLOAT3 GetCurPos() { return XMFLOAT3(m_fxPos, m_fyPos, m_fzPos); }

	void Move(float fElapsedTime);
	void Move(XMFLOAT3& vDirection, float fSpeed);
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


class CMissileObject : public GameObject
{
public:
	int m_cPlayerNumber;
	int damage = 5;
	float m_fMovingSpeed;

	void Move();
	virtual void Move(XMFLOAT3& vDirection, float fSpeed);
};


class CPlayer : public GameObject
{
public:
	CPlayer();
	float m_fFriction;
	int m_nHp = 100;
	CMissileObject* m_pMissiles[8];
	void Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(DWORD Direction, float Distance, bool updateVelocity);
	void Rotate(float x, float y, float z);
	void LaunchMissile();
	void UpdateMissiles();
	void Update(float Distance, bool updateVelocity);

	float m_deltaX{};
	float m_deltaY{};

	unsigned char playerKey{0};
	char playerMouse;
	char activatedMissiles{0};

private:
	unsigned char option0 = 0x01;	// 0000 0001 
	unsigned char option1 = 0x02;	// 0000 0010
	unsigned char option2 = 0x04;	// 0000 0100
	unsigned char option3 = 0x08;	// 0000 1000
	unsigned char option4 = 0x10;	// 0001 0000
	unsigned char option5 = 0x20;	// 0010 0000
	unsigned char option6 = 0x40;	// 0100 0000
	unsigned char option7 = 0x80;	// 1000 0000
};

