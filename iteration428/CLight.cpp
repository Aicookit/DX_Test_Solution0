
#include "CLight.h"
//光照-shader类
CLight::CLight()
{
	// 共同属性 ambient  diffuse specular
	ambientMat = { 1.0f,0.0f,0.0f,1.0f };
	diffuseMat = { 1.0f,0.0f,0.0f,1.0f };
	specularMat= { 1.0f,0.0f,0.0f,1.0f };
}

CLight::~CLight()
{
}

LPD3DXVECTOR4 CLight::GetAmbientMat()
{
	return &ambientMat;
}
LPD3DXVECTOR4 CLight::GetSpecularMat()
{
	return  &specularMat;
}
LPD3DXVECTOR4 CLight::GetDiffuseMat()
{
	return  &diffuseMat;
}

void CLight::SetAmbientMat(float r, float g, float b, float a)
{
	ambientMat.x = r;
	ambientMat.y = g;
	ambientMat.z = b;
	ambientMat.w = a;
}
void CLight::SetDiffuseMat(float r, float g, float b, float a)
{
	diffuseMat.x = r;
	diffuseMat.y = g;
	diffuseMat.z = b;
	diffuseMat.w = a;
}

void CLight::SetSpecularMat(float r, float g, float b, float a)
{
	specularMat.x = r;
	specularMat.y = g;
	specularMat.z = b;
	specularMat.w = a;
}

//方向光
//
CLightDirectional::CLightDirectional()
{
	m_vDirection = { 5.0f,0.0f,1.0f };
	m_eType = LT_DIR;
}
CLightDirectional::~CLightDirectional()
{

}

LPD3DXVECTOR3 CLightDirectional::GetDirection()
{
	return &m_vDirection;      // return to pointerAddress
}

void CLightDirectional::SetDirection(float x, float y, float z)
{
	m_vDirection.x = x;
	m_vDirection.y = y;
	m_vDirection.z = z;
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);   // 归一化处理参数
}

//点光
//
CLightPoint::CLightPoint()
{
	m_vPosition = { 0.0f,1.0f,0.0f };
	m_vAtt = { 0.5f, 0.1f, 0.0f };

	m_eType = LT_POINT;
}
CLightPoint::~CLightPoint()
{

}
LPD3DXVECTOR3 CLightPoint::GetPosition()
{
	return &m_vPosition;  // &取址后 占4字节
}

LPD3DXVECTOR3 CLightPoint::GetAtt()
{
	return &m_vAtt;
}

void CLightPoint::SetPosition(float x, float y, float z)
{
	m_vPosition.x = x;
	m_vPosition.y = y;
	m_vPosition.z = z;
	D3DXVec3Normalize(&m_vPosition, &m_vPosition);   // 归一化处理参数

}

void CLightPoint::SetAtt(float x, float y, float z)
{
	m_vAtt.x = x;
	m_vAtt.y = y;
	m_vAtt.z = z;
	D3DXVec3Normalize(&m_vAtt, &m_vAtt);   // 归一化处理参数

}

//聚光
//
CLightSpot::CLightSpot()
{
	m_vDirection = { 1.0f,0.0f,0.0f };
	m_vRangeAtt = { 1.0f,0.0f,0.0f };

	m_eType = LT_SPOT;
};
CLightSpot::~CLightSpot()
{

}
LPD3DXVECTOR3 CLightSpot::GetDirection()
{
	return &m_vDirection;
}
LPD3DXVECTOR3 CLightSpot::GetRangeAtt()
{
	return &m_vRangeAtt;
}

void CLightSpot::SetDirection(float x, float y, float z)
{
	m_vDirection.x = x;
	m_vDirection.y = y;
	m_vDirection.z = z;
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
}

void CLightSpot::SetRangeAtt(float x, float y, float z)
{
	m_vRangeAtt.x = x;
	m_vRangeAtt.y = y;
	m_vRangeAtt.z = z;
	D3DXVec3Normalize(&m_vRangeAtt, &m_vRangeAtt);  // 归一化处理参数
}


