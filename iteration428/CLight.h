#pragma once
#include "CDevice.h"

enum LIGHT_TYPE
{
	LT_DIR = 0,
	LT_POINT = 0,
	LT_SPOT = 0,
};

class CLight                  
{
protected:
	LIGHT_TYPE m_eType;

public:
	CLight();
	~CLight();
	//mat
	D3DXVECTOR4 ambientMat;
	D3DXVECTOR4 diffuseMat;
	D3DXVECTOR4 specularMat;

	LIGHT_TYPE  GetType() { return m_eType; }
	LPD3DXVECTOR4 GetAmbientMat();
	LPD3DXVECTOR4 GetDiffuseMat();
	LPD3DXVECTOR4 GetSpecularMat();

	void SetAmbientMat(float r, float g, float b, float a);
	void SetDiffuseMat(float r, float g, float b, float a);
	void SetSpecularMat(float r, float g, float b, float a);
};


class CLightDirectional :public CLight   //-平行光
{
	D3DXVECTOR3 m_vDirection;

public:
	CLightDirectional();
	~CLightDirectional();

	LPD3DXVECTOR3 GetDirection();
	void SetDirection(float x, float y, float z);
};


class CLightPoint : public CLight     //-点光
{
	D3DXVECTOR3 m_vPosition;         //位置
	D3DXFLOAT16 m_vRange;            // 照射范围
	D3DXVECTOR3 m_vAtt;              //衰减
public:
	CLightPoint();
	~CLightPoint();

	LPD3DXVECTOR3 GetPosition();
	LPD3DXFLOAT16 GetRange();
	LPD3DXVECTOR3 GetAtt();

	void SetPosition(float x, float y, float z); 
	void SetRange(float fRange);                 
	void SetAtt(float x, float y, float z);      
};



class CLightSpot :public CLight          //-聚光灯
{
	D3DXVECTOR3 m_vDirection;

	D3DXVECTOR3 m_vRangeAtt;
public:
	CLightSpot();
	~CLightSpot();

	LPD3DXVECTOR3 GetDirection();
	LPD3DXVECTOR3 GetRangeAtt();

	void SetDirection(float x, float y, float z);
	void SetRangeAtt(float x, float y, float z);
};



