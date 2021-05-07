#pragma once
//相机类- : 位置、旋转、视野、 ViewMatrix and ProjMatrix setting
//
#include "CDevice.h"
#include "CMaterial.h"


class CCamera
{
public:
	CCamera();
	~CCamera();

protected:
	D3DXVECTOR3 m_vecPosition;
	D3DXVECTOR3 m_vecTarget;
	D3DXVECTOR3 m_vecUp;

	float m_fAngelRange; // 摄像机视角范围 [0 ,1 ]
	float m_fNearClip;    //近裁面 1~
	float m_fFarClip;      //远裁面 1000~

	D3DXMATRIX m_matView = D3DXMATRIX{};
	D3DXMATRIX m_matproj = D3DXMATRIX{};
	D3DXMATRIX m_matVP = D3DXMATRIX{};

public:
	LPD3DXVECTOR3 GetPosition();
	LPD3DXVECTOR3 GetTarget();
	LPD3DXVECTOR3 GetUp();

	void SetPositon(float x, float y, float z);
	void SetTarget(float x, float y, float z);
	void SetUp(float x, float y, float z);

	LPD3DXMATRIX GetViewMatrix();
	LPD3DXMATRIX GetVP() { return &m_matVP; }

	void SetProjMatrix(float  fAngle, float  fNearClip, float  fFarClip);
	LPD3DXMATRIX GetProjMatrix();

	//void SetVPMatrix(CDevice* pDevice, D3DXMATRIX m_matView, D3DXMATRIX m_matproj);

	void Update();  //3更新数据

};
