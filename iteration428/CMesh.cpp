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


//����������װ�� 
HRESULT CMesh::VertexDeclaration(CDevice * pDevice)
{
	if (m_pVertexDecl)
	{
		return true;
	}
	//2���������ɱ�̶������
	D3DVERTEXELEMENT9 VertexDecl[] =
	{
		{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0}, //pos  ����  D3DDECLUSAGE_POSITION��ʾ����λ���Ѿ����任 ��֪ͨͼ�ο���Ҫ����������͵����㴦��׶Σ��任�͹��գ���
		{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0}, //nornal1  ����
		//{0,24,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,1},//nornal 2
		//{0,36,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,2},//nornal3
		//{0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},//��ͼ//����
		//{0,28,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},//nornal		
		D3DDECL_END()  // ��������
	};

	//3����������
	if (FAILED(pDevice->GetD3DDevice()->CreateVertexDeclaration(VertexDecl, &m_pVertexDecl)))  //�ж϶����������ڷ�
	{
		MessageBox(0, "3������������ʧ��", "������������", MB_OK);
		return E_FAIL;
	}
	//4���ö������� �󶨵��豸
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
//��������ģ�ͷ���1 ����������
HRESULT CMesh::CreateTriangle(CDevice * pDevice,float x, float y ,float)
{
	m_iVertexCount = 3;
	m_iFaceCount = 1;

	//�����豸
	LPDIRECT3DDEVICE9 pDevice2 = pDevice->GetD3DDevice();

	//�������㻺�� �� 2���嶥��ṹ
	pDevice2->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, 0);
	pDevice2->CreateIndexBuffer(3 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0);
	CUSTOMVERTEX sourceTriangle_vertices[] =
	{
		////����:λ��/����
		//{-1.0f,-1.0f,0.0f,   0.0f,0.0f},
		//{0.0f,1.0f,0.0f,    1.0f,0.0f},
		//{1.0f,-1.0f,0.0f,    1.0f,1.0f},
		//���㣺λ��/���շ���/��ͼ
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
//��������ģ�ͷ���2 �����ĸ������ƽ��
HRESULT CMesh::CreatePlane(CDevice * pDevice, float x, float y, float z)
{
	m_iVertexCount = 4;
	m_iFaceCount = 2;

	LPDIRECT3DDEVICE9 lpDevice = pDevice->GetD3DDevice();  // ͨ��GetD3DDevice��ȡ�豸���Է����õ�������ͬģ�͵Ĳ�ͬ�豸


	//��ô��ȡ��������m_iVertexCount �� ͼԪ����m_iFaceCount
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

	//2.4 ƽ���ģ��
	CUSTOMVERTEX sourcePlane_vertices[] = {
	/*	{ -1.0f, -1.0f, 0.0f,  0.0f,1.0f},
		{ -1.0f, 1.0f, 0.0f,   0.0f,0.0f},
		{  1.0f, 1.0f, 0.0f,   1.0f,0.0f},
		{  1.0f, -1.0f, 0.0f,  1.0f,1.0f},*/
										{ -1.0f, -1.0f, 0.0f,  -1.0f, -1.0f, -1.0f,    0.0f,1.0f},  //A1 ������  // ���� UV ��ô��Ӧ ����Ե� ����ͼԪ��
										{ -1.0f, 1.0f, 0.0f,   -1.0f, 1.0f,-1.0f,      0.0f,0.0f},   //A2  ������
										{  1.0f, 1.0f, 0.0f,   1.0f, 1.0f, -1.0f,      1.0f,0.0f},     //A3 һ����
										{  1.0f, -1.0f, 0.0f,  1.0f, -1.0f, -1.0f,     1.0f,1.0f},     //A4 ������
	};

	CUSTOMVERTEX * pVertices = NULL;
	if (FAILED(m_pVB->Lock(0, 4 * sizeof(CUSTOMVERTEX), (void **)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, sourcePlane_vertices, 4 * sizeof(CUSTOMVERTEX));
	m_pVB->Unlock();

	WORD * indices = NULL;
	m_pIB->Lock(0, 0, (void **)&indices, 0);

	//��triangleList��ʽ������Ҫn�������εĶ��㣨3n��
	////��һ��������
	//indices[0] = 0;  //0���� ��A1  
	//indices[1] = 1; //1���� ��A2
	//indices[2] = 2;  //2���� ��A3

	////�ڶ���������
	//indices[3] = 0;  //3���� ��A1
	//indices[4] = 2; //4���� ��A3
	//indices[5] = 3; //5���� ��A4
	//��triangleStrip��ʽ��n�����㣬���ܻ���n-2�������Σ�˳�����λ��ƣ�,Ҫ�ҷǹ������ϵĶ�����Ϊ��ʼ����
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 2;

	m_pIB->Unlock();

	return S_OK;

}
////////////////////////////////////////////////////////////////////////////////////////

HRESULT CMesh::CreateSphere2(CDevice *pDevice, int iLongitudes, int iLatitudes) //������������ ������ ������ ����ͼ�� ���ʣ�
{
	int m_iLong = iLongitudes;
	int m_iLati = iLatitudes;
	//���嶥�������������
	m_iVertexCount = (m_iLong + 1)*(m_iLati + 1);   //�������
	m_iFaceCount = (m_iLong + 1)*(m_iLati + 1) * 2;  //�����θ���  : ����������Ϊʲô�Ƕ���������2����
	//m_iFaceCount = m_iVertexCount-2;
	//����1���豸 �������建��
	LPDIRECT3DDEVICE9 pDevice2 = pDevice->GetD3DDevice();
	//�������嶥�㻺�����������
	if (FAILED(pDevice2->CreateVertexBuffer(m_iVertexCount * sizeof(CUSTOMVERTEX), 0, 0, D3DPOOL_MANAGED, &m_pVB, 0)))
		return E_FAIL;
	if (FAILED(pDevice2->CreateIndexBuffer(2 * m_iVertexCount * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0)))
		return E_FAIL;
	//��ס�����еĶ�������
	CUSTOMVERTEX * pVertices;
	if (FAILED(m_pVB->Lock(0, m_iVertexCount * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0)))
		return false;
	m_pVB->Unlock();

	WORD * pIndices;
	INT vIndice = 0;
	if (FAILED(m_pIB->Lock(0, 2 * m_iVertexCount * sizeof(WORD), (void**)&pIndices, 0)))
		return E_FAIL;
	m_pIB->Unlock();

	//��������ʵ�ʶ�������ģ��
	FLOAT rDeltaRingAngle = D3DX_PI / m_iLong;  // ������ߵĽǶ�-����
	FLOAT rDeltaSegAngle = 2.0f * D3DX_PI / m_iLati; // ������Բ�ϵ�γ�߽Ƕ�

	//���������ϵľ���
	for (int i = 0; i < m_iLong + 1; i++)
	{
		FLOAT r0 = sinf(i*rDeltaRingAngle);
		FLOAT y0 = cosf(i*rDeltaRingAngle);
		//���������ϵ�γ��
		for (int j = 0; j < m_iLati + 1; j++)
		{
			FLOAT x0 = r0 * sinf(j * rDeltaSegAngle);
			FLOAT z0 = r0 * cosf(j * rDeltaSegAngle);

			//��Ӷ��㵽�������
			pVertices->x = x0;
			pVertices->y = y0;
			pVertices->z = z0;

			pVertices->u = 1.0f - (FLOAT)(j) / (FLOAT)(m_iLong);
			pVertices->v = (FLOAT)(i) / (FLOAT)(m_iLati);

			pVertices->nx = x0;
			pVertices->ny = y0;
			pVertices->nz = z0;

			pVertices++;

			//�����һ���������ʣ�µ���������
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

	lpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_iVertexCount, 0, m_iFaceCount);//��ÿ��mesh�������ȡ����������ƬԲ����



}

