#include "GameObject.h"

GameObject::GameObject()
{
	m_xmf4x4World = Matrix4x4::Identity();
	SetOOBB(XMFLOAT3(0,0,0), XMFLOAT3(10,10,10), XMFLOAT4(0,0,0,1));
}

void GameObject::Move()
{

}

void GameObject::Rotate(float Pitch, float Yaw, float Roll)
{
	XMMATRIX mtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));
	m_xmf4x4World = Matrix4x4::Multiply(mtxRotate, m_xmf4x4World);

	//UpdateTransform(NULL);

}

void GameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;

	m_fxPos = x;
	m_fyPos = y;
	m_fzPos = z;

	m_xmf3Position = XMFLOAT3{ x,y,z };
	
	//m_xmOOBB.Center = { x,y,z };
	m_xmOOBB = BoundingOrientedBox{ m_xmf3Position, XMFLOAT3(10,10,10), XMFLOAT4(0,0,0,1) };
}

void GameObject::SetActive(bool active)
{
	m_bActive = active;
}

bool GameObject::GetActive()
{
	return m_bActive;
}

void CPlayer::Move(const XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	m_fOldxPos = m_xmf3Position.x;
	m_fOldyPos = m_xmf3Position.y;
	m_fOldzPos = m_xmf3Position.z;

	m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);

	m_fxPos = m_xmf3Position.x;
	m_fyPos = m_xmf3Position.y;
	m_fzPos = m_xmf3Position.z;
}

void CPlayer::Move(DWORD Direction, float Distance, bool updateVelocity)
{
	if (Direction)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (Direction & DIR_FORWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, Distance);
		if (Direction & DIR_BACKWARD) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -Distance);
		if (Direction & DIR_RIGHT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, Distance);
		if (Direction & DIR_LEFT) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -Distance);
		if (Direction & DIR_UP) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, Distance);
		if (Direction & DIR_DOWN) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Up, -Distance);
		Move(xmf3Shift, updateVelocity);
	}
}

void CPlayer::Move(unsigned char key, float Distance, bool updateVelocity)
{
	RecalculateLook();
	RecalculateRight();

	if (key)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0.f, 0.f, 0.f);
		
		if (key & option0) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, Distance);
		if (key & option1) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Look, -Distance);
		if (key & option2) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, -Distance);
		if (key & option3) xmf3Shift = Vector3::Add(xmf3Shift, m_xmf3Right, Distance);


		Move(xmf3Shift, updateVelocity);

		//m_xmOOBB.Center = GetCurPos();
		m_xmOOBB = BoundingOrientedBox{ m_xmf3Position, XMFLOAT3(10,10,10), XMFLOAT4(0,0,0,1) };
	}
}