#pragma once
#include "CDevice.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CLight.h"
#include "CCamera.h"

//CModel is used for organize Mesh and Material

class CModel
{
	CMesh * m_pMesh;
	CMaterial * m_pMaterial;

	D3DXMATRIX m_mWorld;  //Local Coor converts to World Coordinates

	D3DXVECTOR3 m_vPos;   //local Position
	D3DXVECTOR3 m_vRotate;
	D3DXVECTOR3 m_vScale;

public:
	CModel();
	~CModel();

public:
	void SetMesh(CMesh * pMesh);
	void SetMaterial(CMaterial * pMaterial);

	void SetPos(float x, float y, float z); //local Position
	void SetRotate(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void Render(CDevice * pDevice,CLight * pLight, CCamera * pCamera);

};

