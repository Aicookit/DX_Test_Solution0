#include "CMaterial.h"

CMaterial::CMaterial()
{
	m_pTexture = NULL;

	m_pVShader = NULL;
	m_pPShader = NULL;
	m_pVSConstTable = NULL;
	m_pPSconstTable = NULL;

}

CMaterial::~CMaterial()
{
	SAFE_RELEASE(m_pTexture);//pDevice->GetD3DDevice()->SetTexture(0, NULL);//���� ��Ⱦ��� �Ƴ�
	SAFE_RELEASE(m_pVShader);
	SAFE_RELEASE(m_pPShader);
	SAFE_RELEASE(m_pVSConstTable);
	SAFE_RELEASE(m_pPSconstTable);
}

LPDIRECT3DTEXTURE9 CMaterial::GetTexture()
{
	return m_pTexture;
}
LPDIRECT3DVERTEXSHADER9 CMaterial::GetVShader()
{
	return m_pVShader;
}
LPDIRECT3DPIXELSHADER9 CMaterial::GetPShader()
{
	return m_pPShader;
}

//Mat load texture function
HRESULT CMaterial::LoadTexture(CDevice * pDevice, const char *pTextureName)
{
	if (FAILED(D3DXCreateTextureFromFile(pDevice->GetD3DDevice(), pTextureName, &m_pTexture)))
	{
		MessageBox(NULL, "�Ҳ�������ͼ", "D3DXCreateTextureFromFile()", MB_OK);
		return E_FAIL;
	}
	return S_OK;

}

//  VS  LoadShader func include compile and create Shader
HRESULT CMaterial::LoadVShader(CDevice*pDevice, const char*pVShaderName) //����VS
{
	LPD3DXBUFFER shader = NULL;
	LPD3DXBUFFER errorBuffer = NULL;

	//---4-1 ����VSshader
	if (FAILED(D3DXCompileShaderFromFile(pVShaderName, 0, 0, "Main", "vs_2_0", D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &shader, &errorBuffer, &m_pVSConstTable)))
	{
		if (errorBuffer)
		{
			const char * szp = (const char *)errorBuffer->GetBufferPointer();   // ������󶥵���Ϣ
			MessageBox(NULL, szp, "����VSshaderʧ��", MB_OK);
		}
		else
			MessageBox(NULL, "�����޷��ҵ�VShader�ļ�", "����VSshaderʧ��", MB_OK);
		return E_FAIL;
	}

	//--- 4-2����VSshader
	if (FAILED(pDevice->GetD3DDevice()->CreateVertexShader((DWORD*)shader->GetBufferPointer(), &m_pVShader)))
	{
		MessageBox(NULL, "����VSshaderʧ��", "����VShader", MB_OK);
		return E_FAIL;
	}
	//����shader���ͷ�
	if (shader)
	{
		shader->Release();
	}
	if (errorBuffer)
	{
		errorBuffer->Release();
	}
	// ��ֵǰ��Ĭ��
	m_pVSConstTable->SetDefaults(pDevice->GetD3DDevice());

	return S_OK;
}

//   PS
HRESULT CMaterial::LoadPShader(CDevice*pDevice, const char*pPSshaderName) // ����PS
{
	HRESULT hr;
	LPD3DXBUFFER shader = NULL;
	LPD3DXBUFFER errorBuffer = NULL;

	//����PS
	hr = D3DXCompileShaderFromFile(pPSshaderName, 0, 0, "Main", "ps_2_0", D3DXSHADER_DEBUG, &shader, &errorBuffer, &m_pPSconstTable);
	if (hr != S_OK)
	{
		if (errorBuffer)
		{
			const char * szp = (const char *)errorBuffer->GetBufferPointer();   // ������󶥵���Ϣ
			MessageBox(NULL, szp, "����PSshaderʧ��", MB_OK);
		}
		else
			MessageBox(NULL, "�����޷��ҵ�PShader�ļ�", "����PSshaderʧ��", MB_OK);
		return E_FAIL;
	}
	//����PS
	hr = pDevice->GetD3DDevice()->CreatePixelShader((DWORD*)shader->GetBufferPointer(), &m_pPShader);
	if (FAILED(hr))
		return false;

	//+4  ��PShader�б�����ֵǰ��ΪĬ��
	m_pPSconstTable->SetDefaults(pDevice->GetD3DDevice());

	if (shader)
	{
		shader->Release();
	}
	if (errorBuffer)
	{
		errorBuffer->Release();
	}
	return S_OK;
}

/// Set Shader value
//����shader�еı���:matrix,float4,float...���и�ֵ---����Ⱦ��ִ��(Render)
HRESULT CMaterial::SetMatrix(CDevice*pDevice, const char*pMatrixName, D3DXMATRIX * mMatrixValue)  //SetMatrix���豸�� shader�б������ַ������û�Ҫ���õ�ֵ��
{
	D3DXHANDLE VSHandle = m_pVSConstTable->GetConstantByName(0, pMatrixName);
	D3DXHANDLE PSHandle = m_pPSconstTable->GetConstantByName(0, pMatrixName);
	if (VSHandle == NULL && PSHandle == NULL)
	{
		MessageBox(NULL, "shderHandle is Empty", "��ȡshade����", MB_OK);
		return E_FAIL;
	}
	m_pVSConstTable->SetMatrix(pDevice->GetD3DDevice(), VSHandle, mMatrixValue); // (�豸����ȡshader�����ľ����Ҫ���õ�ֵ��
	m_pPSconstTable->SetMatrix(pDevice->GetD3DDevice(), PSHandle, mMatrixValue);

	return S_OK;
}
HRESULT CMaterial::SetVector(CDevice*pDevice, const char*pHandle, D3DXVECTOR4 * fVector)
{
	D3DXHANDLE VSHandle = m_pVSConstTable->GetConstantByName(0, pHandle);
	//D3DXHANDLE PSHandle = m_pPSconstTable->GetConstantByName(0, pHandle);
	if (VSHandle == NULL)
	{
		MessageBox(NULL, "shderHandle is Empty", "��ȡshader����", MB_OK);
		return E_FAIL;
	}
	m_pVSConstTable->SetVector(pDevice->GetD3DDevice(), VSHandle, fVector);
	//m_pPSconstTable->SetVector(pDevice->GetD3DDevice(), PSHandle, fVector);

	return S_OK;
}
HRESULT CMaterial::SetFloat(CDevice*pDevice, const char*pHandle, FLOAT  fFloat)
{
	D3DXHANDLE VSHandle = m_pVSConstTable->GetConstantByName(0, pHandle);
	D3DXHANDLE PSHandle = m_pPSconstTable->GetConstantByName(0, pHandle);
	if (VSHandle == NULL && PSHandle == NULL)
	{
		MessageBox(NULL, "shderHandle is Empty", "��ȡshader����", MB_OK);
		return E_FAIL;
	}
	m_pVSConstTable->SetFloat(pDevice->GetD3DDevice(), VSHandle, fFloat);
	m_pPSconstTable->SetFloat(pDevice->GetD3DDevice(), PSHandle, fFloat);

	return S_OK;
}

HRESULT CMaterial::SetFloatArray(CDevice * pDevice, const char * pHandle, FLOAT * pFloat, int iCount)
{
	D3DXHANDLE VSHandle = m_pVSConstTable->GetConstantByName(0, pHandle);
	D3DXHANDLE PSHandle = m_pPSconstTable->GetConstantByName(0, pHandle);
	if (VSHandle == NULL && PSHandle == NULL)
	{
		MessageBox(NULL, "shderHandle is Empty", "��ȡshader����", MB_OK);
		return E_FAIL;
	}
	m_pVSConstTable->SetFloatArray(pDevice->GetD3DDevice(), VSHandle, pFloat, iCount);
	m_pPSconstTable->SetFloatArray(pDevice->GetD3DDevice(), PSHandle, pFloat, iCount);

	return S_OK;
}


//Mat set texture, shader and worldMatrix bind to Device  in Render for update each fram
HRESULT CMaterial::Apply(CDevice * pDevice, LPD3DXMATRIX lpWorld, CLight*pLight, CCamera * pCamera)
{
	if (FAILED(pDevice->GetD3DDevice()->SetTexture(0, m_pTexture)))
	{
		MessageBox(NULL, "������ͼʧ��", "ʹ����ͼ��..", MB_OK); // texture��������
		return E_FAIL;
	}
	if (FAILED(pDevice->GetD3DDevice()->SetVertexShader(m_pVShader))) //VS�������� 
	{
		MessageBox(NULL, "����VSʧ��", "ʹ��shader��..", MB_OK);
		return E_FAIL;
	}
	if (FAILED(pDevice->GetD3DDevice()->SetPixelShader(m_pPShader)))  // PS����������
	{
		MessageBox(NULL, "����PSʧ��", "ʹ��shader��..", MB_OK);
		return E_FAIL;
	}

	//shader �� Mat-Light ͨ�� ��ֵ
	//SetFloatArray(pDevice, "Ambient", *pLight->GetAmbient(), 3);  //CTable->SetVector()������ΪVector4��
	//SetFloatArray(pDevice, "Diffuse", *pLight->GetDiffuse(), 3);
	//SetFloatArray(pDevice, "Specular", *pLight->GetSpecular(), 3);

	D3DXMATRIX wvp = *lpWorld * *pCamera->GetVP();   // Get WVP by compute then update the result of WVP under Render()
	SetMatrix(pDevice, "WorldViewMatrix", &wvp);  // assign WVP value to the variable of "WorldViewMatrix" in VS

	//�����
	if (pLight->GetType() == LT_DIR)
	{
		CLightDirectional * pDir = (CLightDirectional *)pLight;  //�ѻ���pLightǿ��תΪ ������ƽ�е�
		SetVector(pDevice, "AmbientMat", pDir->GetAmbientMat());
		SetVector(pDevice, "DiffuseMat", pDir->GetDiffuseMat());
		SetVector(pDevice, "LightDirection", pDir->GetAmbientMat());
		//SetFloatArray(pDevice, "LightDirection", *pDir->GetDirection(), 3);     // �Է���Vshader�еı���LightDirection��ֵ��*pDir->GetDirection()
	}
	//���
	else if (pLight->GetType() == LT_POINT)
	{
		CLightPoint * pPoint = (CLightPoint *)pLight;

		SetFloat(pDevice, "Range", *pPoint->GetRange());

	}
	else if (pLight->GetType() == LT_SPOT)
	{
		CLightSpot * pSpot = (CLightSpot *)pLight;
		//SetVector(pDevice, "LightSpot", &pSpot->GetDirection());
		//SetFloatArray(pDevice, "LightDir", *pSpot->GetDirection(), 3);  //(�豸��shader�б�����ĳ���ջ�ȡ���Է����� float4 תΪfloat3)

		//�������Spot����

	}

	return S_OK;
}
