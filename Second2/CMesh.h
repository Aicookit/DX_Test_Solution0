#pragma once  // 三层
#include "CDevice.h"

//#define D3DFVF_CUSTOMVERTEX		(D3DFVF_XYZ | D3DFVF_DIFFUSE)

class CMesh
{
	//模型mesh顶点缓存和索引字段
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	LPDIRECT3DVERTEXDECLARATION9 	m_pVertexDecl;

	int m_iVertexCount;
	int m_iFaceCount;

public:
	CMesh();
	~CMesh();

	HRESULT VertexDeclaration(CDevice * pDevice);  //~ 2 use VertexDecl func

	//获取顶点缓存变量和索引属性
	LPDIRECT3DVERTEXBUFFER9 GetVB();
	LPDIRECT3DINDEXBUFFER9 GetIB();
	                                                                //HRESULT LoadFile(CDevice * pDevice, const char * szpFileName);
	HRESULT CreatePlane(CDevice * pDevice,float x , float y , float z );    // 创建几何模型平面
	HRESULT CreateTriangle(CDevice * pDevice, float x, float y, float);  // 创建几何模型三角
	HRESULT CreateSphere(CDevice * pDevice,int longitudeCount,int latitudeCount);   //// 创建几何模型球体
	HRESULT CreateSphere2(CDevice * pDevice, int iLonggitudeCout, int iLatitudeCout);//球体模型二
	void GetMesh(CDevice * pDevice);              //调用设备渲染模型
};