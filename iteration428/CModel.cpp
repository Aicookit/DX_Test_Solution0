#include "CModel.h"


CModel::CModel()
{
	m_pMaterial = NULL;
	m_pMesh = NULL;
}

CModel::~CModel()
{
	if (m_pMaterial != NULL)
	{
		delete m_pMaterial;
		m_pMaterial = NULL;
	}
	if (m_pMesh != NULL)
	{
		delete m_pMesh;
		m_pMesh = NULL;
	}
}

void CModel::SetMesh(CMesh * pMesh)
{
	m_pMesh = pMesh;
}

void CModel::SetMaterial(CMaterial * pMaterial)
{
	m_pMaterial = pMaterial;
}

void CModel::SetPos(float x, float y, float z)
{
	m_vPos.x = x;
	m_vPos.y = y;
	m_vPos.z = z;
}

void CModel::SetRotate(float x, float y, float z)
{
	m_vRotate.x = x;
	m_vRotate.y = y;
	m_vRotate.z = z;
}

void CModel::SetScale(float x, float y, float z)
{
	m_vScale.x = x;
	m_vScale.y = y;
	m_vScale.z = z;
}

void CModel::Render(CDevice * pDevice, CLight * pLight, CCamera * pCamera)
{
	D3DXMATRIX translate, rotate, scale;      // 对local coord移动、旋转、缩放 从而利用矩阵的移动旋转缩放方法控制model在World coordinate中的移动。。。
	D3DXMatrixTranslation(&translate, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixRotationYawPitchRoll(&rotate, m_vRotate.x, m_vRotate.y, m_vRotate.z);
	D3DXMatrixScaling(&scale, m_vScale.x, m_vScale.y, m_vScale.z);

	m_mWorld = translate * rotate * scale;

	m_pMesh->GetMesh(pDevice);

	m_pMaterial->Apply(pDevice, &m_mWorld, pLight, pCamera);  // Apply(CDevice *pDevice, LPD3DXMATRIX lpWorld, CLight * pLight) apply中包含了模型本地到世界的转换，光照shader变量的设值。。

}
