#pragma once  // ����
#include "CDevice.h"

//#define D3DFVF_CUSTOMVERTEX		(D3DFVF_XYZ | D3DFVF_DIFFUSE)

class CMesh
{
	//ģ��mesh���㻺��������ֶ�
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	LPDIRECT3DVERTEXDECLARATION9 	m_pVertexDecl;

	int m_iVertexCount;
	int m_iFaceCount;

public:
	CMesh();
	~CMesh();

	HRESULT VertexDeclaration(CDevice * pDevice);  //~ 2 use VertexDecl func

	//��ȡ���㻺���������������
	LPDIRECT3DVERTEXBUFFER9 GetVB();
	LPDIRECT3DINDEXBUFFER9 GetIB();


	HRESULT CreateTriangle(CDevice * pDevice,float x, float y, float z);  // ��ʼ������ģ��
	HRESULT CreatePlane(CDevice * pDevice,float x , float y , float z );    // ��������ģ��ƽ��
	HRESULT CreateSphere2(CDevice * pDevice, int iLonggitudeCout, int iLatitudeCout);//����ģ�Ͷ�
	void GetMesh(CDevice * pDevice);              //�����豸��Ⱦģ��
};