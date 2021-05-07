#include "CCamera.h"

CCamera::CCamera()
{
	m_vecPosition = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	m_vecTarget= D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_vecUp= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_fAngelRange = 4.0f; 
	m_fNearClip = 1.0f;  
	m_fFarClip = 1000.0f;      
}

CCamera::~CCamera()
{

}

LPD3DXVECTOR3 CCamera::GetPosition()
{
	return &m_vecPosition;
}

LPD3DXVECTOR3 CCamera::GetTarget()
{
	return &m_vecTarget;
}

LPD3DXVECTOR3 CCamera::GetUp()
{
	return &m_vecUp;
}

void CCamera::SetPositon(float x, float y, float z)
{
	m_vecPosition = D3DXVECTOR3{ x,y,z };
	D3DXMatrixLookAtLH(&m_matView, &m_vecPosition, &m_vecTarget, &m_vecUp);
}

void CCamera::SetTarget(float x, float y, float z)
{
	m_vecTarget = D3DXVECTOR3(x, y, z);
	D3DXMatrixLookAtLH(&m_matView, &m_vecPosition, &m_vecTarget, &m_vecUp);
}

void CCamera::SetUp(float x, float y, float z)
{
	m_vecUp = D3DXVECTOR3(x, y, z);
	D3DXMatrixLookAtLH(&m_matView, &m_vecPosition, &m_vecTarget, &m_vecUp);
}

void CCamera::SetProjMatrix(float fAngle, float fNearClip, float fFarClip)  // 1 set() 数据并comput
{
	m_fAngelRange = fAngle;  
	m_fNearClip = fNearClip;
	m_fFarClip = fFarClip;
	D3DXMatrixPerspectiveFovLH(&m_matproj, D3DX_PI / m_fAngelRange, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, m_fNearClip, m_fFarClip);
}

LPD3DXMATRIX CCamera::GetViewMatrix()
{
	//D3DXMatrixLookAtLH(&m_matView, &m_vecPosition, &m_vecTarget, &m_vecUp);  //Set属性就包含设值并运算
	return &m_matView;
}

LPD3DXMATRIX CCamera::GetProjMatrix()    //2 get数据并返回
{
	return &m_matproj;
}

void CCamera::Update()  //3更新数据
{
	m_matVP = m_matView * m_matproj;
}
