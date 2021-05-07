#include "CMesh.h"

CMesh::CMesh()
{
	m_pVB = NULL;
	m_pIB = NULL;
	m_pVertexDecl = NULL;  // `1 Vertex decl

}
CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVertexDecl);
}


struct  CUSTOMVERTEX
{
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
	FLOAT u, v;
};


//顶点声明封装： 
HRESULT CMesh::VertexDeclaration(CDevice * pDevice)
{
	if (m_pVertexDecl)
	{
		return true;
	}
	//2定义描述可编程顶点分量
	D3DVERTEXELEMENT9 VertexDecl[] =
	{
		{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0}, //pos  坐标  D3DDECLUSAGE_POSITION表示顶点位置已经被变换 它通知图形卡不要把这个顶点送到顶点处理阶段（变换和光照）。
		{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0}, //nornal1  法线
		//{0,24,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,1},//nornal 2
		//{0,36,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,2},//nornal3
		//{0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},//贴图//纹理
		//{0,28,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},//nornal		
		D3DDECL_END()  // 结束声明
	};

	//3创建顶点声
	if (FAILED(pDevice->GetD3DDevice()->CreateVertexDeclaration(VertexDecl, &m_pVertexDecl)))  //判断顶点声明存在否
	{
		MessageBox(0, "3创建顶点声明失败", "创建顶点声明", MB_OK);
		return E_FAIL;
	}
	//4启用顶点声明 绑定到设备
	pDevice->GetD3DDevice()->SetVertexDeclaration(m_pVertexDecl);

	return S_OK;
}

LPDIRECT3DVERTEXBUFFER9 CMesh::GetVB()
{
	return m_pVB;
}
LPDIRECT3DINDEXBUFFER9 CMesh::GetIB()
{
	return m_pIB;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//创建几何模型方法1 绘制三角形
HRESULT CMesh::CreateTriangle(CDevice * pDevice,float x, float y ,float)
{
	m_iVertexCount = 3;
	m_iFaceCount = 1;

	//创建设备
	LPDIRECT3DDEVICE9 pDevice2 = pDevice->GetD3DDevice();

	//声明顶点缓存 和 2定义顶点结构
	pDevice2->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, 0);
	pDevice2->CreateIndexBuffer(3 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);
	CUSTOMVERTEX sourceTriangle_vertices[] =
	{
		////顶点:位置/法线
		//{-1.0f,-1.0f,0.0f,   0.0f,0.0f},
		//{0.0f,1.0f,0.0f,    1.0f,0.0f},
		//{1.0f,-1.0f,0.0f,    1.0f,1.0f},
		//顶点：位置/光照法线/贴图
		{-1.0f,-1.0f,0.0f,  -1.0f,-1.0f,1.0f,   0.0f,0.0f},
		{0.0f,1.0f,0.0f,    0.0f,1.0f,1.0f,     1.0f,0.0f},
		{1.0f,-1.0f,0.0f,   1.0f,-1.0f,1.0f,    1.0f,1.0f},
	};
	CUSTOMVERTEX * pVertices = NULL;
	if (FAILED(m_pVB->Lock(0, 3 * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, sourceTriangle_vertices, sizeof(CUSTOMVERTEX) * 3);
	m_pVB->Unlock();

	WORD *Indices = NULL;
	if (FAILED(m_pIB->Lock(0, 3 * sizeof(WORD), (void**)&Indices, 0)))
		return E_FAIL;
	Indices[0] = 0; Indices[1] = 1; Indices[2] = 2;
	m_pIB->Unlock();

	return S_OK;
}
//创建几何模型方法2 绘制四个顶点的平面
HRESULT CMesh::CreatePlane(CDevice * pDevice, float x, float y, float z)
{
	m_iVertexCount = 4;
	m_iFaceCount = 2;

	LPDIRECT3DDEVICE9 lpDevice = pDevice->GetD3DDevice();  // 通过GetD3DDevice获取设备属性方法得到创建不同模型的不同设备


	//怎么获取顶点数量m_iVertexCount 和 图元数量m_iFaceCount
	lpDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pVB,
		0);

	lpDevice->CreateIndexBuffer(4 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		0);

	//2.4 平面的模型
	CUSTOMVERTEX sourcePlane_vertices[] = {
	/*	{ -1.0f, -1.0f, 0.0f,  0.0f,1.0f},
		{ -1.0f, 1.0f, 0.0f,   0.0f,0.0f},
		{  1.0f, 1.0f, 0.0f,   1.0f,0.0f},
		{  1.0f, -1.0f, 0.0f,  1.0f,1.0f},*/
										{ -1.0f, -1.0f, 0.0f,  -1.0f, -1.0f, -1.0f,    0.0f,1.0f},  //A1 三象限  // 纹理 UV 怎么对应 到相对的 顶点图元上
										{ -1.0f, 1.0f, 0.0f,   -1.0f, 1.0f,-1.0f,      0.0f,0.0f},   //A2  四象限
										{  1.0f, 1.0f, 0.0f,   1.0f, 1.0f, -1.0f,      1.0f,0.0f},     //A3 一象限
										{  1.0f, -1.0f, 0.0f,  1.0f, -1.0f, -1.0f,     1.0f,1.0f},     //A4 二象限
	};

	CUSTOMVERTEX * pVertices = NULL;
	if (FAILED(m_pVB->Lock(0, 4 * sizeof(CUSTOMVERTEX), (void **)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, sourcePlane_vertices, 4 * sizeof(CUSTOMVERTEX));
	m_pVB->Unlock();

	WORD * indices = NULL;
	m_pIB->Lock(0, 0, (void **)&indices, 0);

	//用triangleList方式绘制需要n个三角形的顶点（3n）
	////第一个三角形
	//indices[0] = 0;  //0索引 点A1  
	//indices[1] = 1; //1索引 点A2
	//indices[2] = 2;  //2索引 点A3

	////第二个三角形
	//indices[3] = 0;  //3索引 点A1
	//indices[4] = 2; //4索引 点A3
	//indices[5] = 3; //5索引 点A4
	//用triangleStrip方式有n个顶点，就能绘制n-2个三角形（顺序蛇形绘制）,要找非公共边上的顶点作为开始索引
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 2;

	m_pIB->Unlock();

	return S_OK;

}
////////////////////////////////////////////////////////////////////////////////////////

HRESULT CMesh::CreateSphere2(CDevice *pDevice, int iLongitudes, int iLatitudes) //（包含：顶点 、法线 、光照 、贴图、 材质）
{
	int m_iLong = iLongitudes;
	int m_iLati = iLatitudes;
	//定义顶点的数量和面数
	m_iVertexCount = (m_iLong + 1)*(m_iLati + 1);   //顶点个数
	m_iFaceCount = (m_iLong + 1)*(m_iLati + 1) * 2;  //三角形个数  : 三角形面数为什么是顶点数量的2倍？
	//m_iFaceCount = m_iVertexCount-2;
	//生成1个设备 创建球体缓存
	LPDIRECT3DDEVICE9 pDevice2 = pDevice->GetD3DDevice();
	//创建球体顶点缓存和索引缓存
	if (FAILED(pDevice2->CreateVertexBuffer(m_iVertexCount * sizeof(CUSTOMVERTEX), 0, 0, D3DPOOL_MANAGED, &m_pVB, 0)))
		return E_FAIL;
	if (FAILED(pDevice2->CreateIndexBuffer(2 * m_iVertexCount * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0)))
		return E_FAIL;
	//锁住缓存中的顶点数据
	CUSTOMVERTEX * pVertices;
	if (FAILED(m_pVB->Lock(0, m_iVertexCount * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0)))
		return false;
	m_pVB->Unlock();

	WORD * pIndices;
	INT vIndice = 0;
	if (FAILED(m_pIB->Lock(0, 2 * m_iVertexCount * sizeof(WORD), (void**)&pIndices, 0)))
		return E_FAIL;
	m_pIB->Unlock();

	//创建球体实际顶点数据模型
	FLOAT rDeltaRingAngle = D3DX_PI / m_iLong;  // 求出经线的角度-常量
	FLOAT rDeltaSegAngle = 2.0f * D3DX_PI / m_iLati; // 求出半个圆上的纬线角度

	//生成球面上的经线
	for (int i = 0; i < m_iLong + 1; i++)
	{
		FLOAT r0 = sinf(i*rDeltaRingAngle);
		FLOAT y0 = cosf(i*rDeltaRingAngle);
		//生成球面上的纬线
		for (int j = 0; j < m_iLati + 1; j++)
		{
			FLOAT x0 = r0 * sinf(j * rDeltaSegAngle);
			FLOAT z0 = r0 * cosf(j * rDeltaSegAngle);

			//添加顶点到球体带上
			pVertices->x = x0;
			pVertices->y = y0;
			pVertices->z = z0;

			pVertices->u = 1.0f - (FLOAT)(j) / (FLOAT)(m_iLong);
			pVertices->v = (FLOAT)(i) / (FLOAT)(m_iLati);

			pVertices->nx = x0;
			pVertices->ny = y0;
			pVertices->nz = z0;

			pVertices++;

			//给最后一条经线添加剩下的两个顶点
			if (i != m_iLati)
			{
				*pIndices = vIndice;
				pIndices++;

				*pIndices = vIndice + (WORD)(m_iLati + 1);
				pIndices++;

				vIndice++;
			}
		}
	}
	return S_OK;
}


VOID CMesh::GetMesh(CDevice * pDevice)
{
	LPDIRECT3DDEVICE9 lpDevice = pDevice->GetD3DDevice();

	lpDevice->SetStreamSource(0, m_pVB, 0, sizeof(CUSTOMVERTEX));

	
	//lpDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	lpDevice->SetIndices(m_pIB);

	lpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_iVertexCount, 0, m_iFaceCount);//在每个mesh方法里获取顶点数量和片圆数量



}

