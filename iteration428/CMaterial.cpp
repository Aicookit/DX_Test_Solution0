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
	SAFE_RELEASE(m_pTexture);//pDevice->GetD3DDevice()->SetTexture(0, NULL);//纹理 渲染完后 移除
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
		MessageBox(NULL, "找不到该贴图", "D3DXCreateTextureFromFile()", MB_OK);
		return E_FAIL;
	}
	return S_OK;

}

//  VS  LoadShader func include compile and create Shader
HRESULT CMaterial::LoadVShader(CDevice*pDevice, const char*pVShaderName) //加载VS
{
	LPD3DXBUFFER shader = NULL;
	LPD3DXBUFFER errorBuffer = NULL;

	//---4-1 编译VSshader
	if (FAILED(D3DXCompileShaderFromFile(pVShaderName, 0, 0, "Main", "vs_2_0", D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, &shader, &errorBuffer, &m_pVSConstTable)))
	{
		if (errorBuffer)
		{
			const char * szp = (const char *)errorBuffer->GetBufferPointer();   // 报告错误顶点信息
			MessageBox(NULL, szp, "编译VSshader失败", MB_OK);
		}
		else
			MessageBox(NULL, "可能无法找到VShader文件", "编译VSshader失败", MB_OK);
		return E_FAIL;
	}

	//--- 4-2创建VSshader
	if (FAILED(pDevice->GetD3DDevice()->CreateVertexShader((DWORD*)shader->GetBufferPointer(), &m_pVShader)))
	{
		MessageBox(NULL, "创建VSshader失败", "创建VShader", MB_OK);
		return E_FAIL;
	}
	//创建shader后释放
	if (shader)
	{
		shader->Release();
	}
	if (errorBuffer)
	{
		errorBuffer->Release();
	}
	// 赋值前设默认
	m_pVSConstTable->SetDefaults(pDevice->GetD3DDevice());

	return S_OK;
}

//   PS
HRESULT CMaterial::LoadPShader(CDevice*pDevice, const char*pPSshaderName) // 加载PS
{
	HRESULT hr;
	LPD3DXBUFFER shader = NULL;
	LPD3DXBUFFER errorBuffer = NULL;

	//编译PS
	hr = D3DXCompileShaderFromFile(pPSshaderName, 0, 0, "Main", "ps_2_0", D3DXSHADER_DEBUG, &shader, &errorBuffer, &m_pPSconstTable);
	if (hr != S_OK)
	{
		if (errorBuffer)
		{
			const char * szp = (const char *)errorBuffer->GetBufferPointer();   // 报告错误顶点信息
			MessageBox(NULL, szp, "编译PSshader失败", MB_OK);
		}
		else
			MessageBox(NULL, "可能无法找到PShader文件", "编译PSshader失败", MB_OK);
		return E_FAIL;
	}
	//创建PS
	hr = pDevice->GetD3DDevice()->CreatePixelShader((DWORD*)shader->GetBufferPointer(), &m_pPShader);
	if (FAILED(hr))
		return false;

	//+4  对PShader中变量赋值前设为默认
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
//访问shader中的变量:matrix,float4,float...进行赋值---在渲染中执行(Render)
HRESULT CMaterial::SetMatrix(CDevice*pDevice, const char*pMatrixName, D3DXMATRIX * mMatrixValue)  //SetMatrix（设备， shader中变量的字符串，用户要设置的值）
{
	D3DXHANDLE VSHandle = m_pVSConstTable->GetConstantByName(0, pMatrixName);
	D3DXHANDLE PSHandle = m_pPSconstTable->GetConstantByName(0, pMatrixName);
	if (VSHandle == NULL && PSHandle == NULL)
	{
		MessageBox(NULL, "shderHandle is Empty", "获取shade变量", MB_OK);
		return E_FAIL;
	}
	m_pVSConstTable->SetMatrix(pDevice->GetD3DDevice(), VSHandle, mMatrixValue); // (设备，获取shader变量的句柄，要设置的值）
	m_pPSconstTable->SetMatrix(pDevice->GetD3DDevice(), PSHandle, mMatrixValue);

	return S_OK;
}
HRESULT CMaterial::SetVector(CDevice*pDevice, const char*pHandle, D3DXVECTOR4 * fVector)
{
	D3DXHANDLE VSHandle = m_pVSConstTable->GetConstantByName(0, pHandle);
	//D3DXHANDLE PSHandle = m_pPSconstTable->GetConstantByName(0, pHandle);
	if (VSHandle == NULL)
	{
		MessageBox(NULL, "shderHandle is Empty", "获取shader变量", MB_OK);
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
		MessageBox(NULL, "shderHandle is Empty", "获取shader变量", MB_OK);
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
		MessageBox(NULL, "shderHandle is Empty", "获取shader标量", MB_OK);
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
		MessageBox(NULL, "设置贴图失败", "使用贴图中..", MB_OK); // texture设置启用
		return E_FAIL;
	}
	if (FAILED(pDevice->GetD3DDevice()->SetVertexShader(m_pVShader))) //VS设置启用 
	{
		MessageBox(NULL, "设置VS失败", "使用shader中..", MB_OK);
		return E_FAIL;
	}
	if (FAILED(pDevice->GetD3DDevice()->SetPixelShader(m_pPShader)))  // PS的设置启用
	{
		MessageBox(NULL, "设置PS失败", "使用shader中..", MB_OK);
		return E_FAIL;
	}

	//shader 与 Mat-Light 通信 设值
	//SetFloatArray(pDevice, "Ambient", *pLight->GetAmbient(), 3);  //CTable->SetVector()参数中为Vector4，
	//SetFloatArray(pDevice, "Diffuse", *pLight->GetDiffuse(), 3);
	//SetFloatArray(pDevice, "Specular", *pLight->GetSpecular(), 3);

	D3DXMATRIX wvp = *lpWorld * *pCamera->GetVP();   // Get WVP by compute then update the result of WVP under Render()
	SetMatrix(pDevice, "WorldViewMatrix", &wvp);  // assign WVP value to the variable of "WorldViewMatrix" in VS

	//方向光
	if (pLight->GetType() == LT_DIR)
	{
		CLightDirectional * pDir = (CLightDirectional *)pLight;  //把基类pLight强制转为 派生类平行灯
		SetVector(pDevice, "AmbientMat", pDir->GetAmbientMat());
		SetVector(pDevice, "DiffuseMat", pDir->GetDiffuseMat());
		SetVector(pDevice, "LightDirection", pDir->GetAmbientMat());
		//SetFloatArray(pDevice, "LightDirection", *pDir->GetDirection(), 3);     // 对方向Vshader中的变量LightDirection赋值：*pDir->GetDirection()
	}
	//点光
	else if (pLight->GetType() == LT_POINT)
	{
		CLightPoint * pPoint = (CLightPoint *)pLight;

		SetFloat(pDevice, "Range", *pPoint->GetRange());

	}
	else if (pLight->GetType() == LT_SPOT)
	{
		CLightSpot * pSpot = (CLightSpot *)pLight;
		//SetVector(pDevice, "LightSpot", &pSpot->GetDirection());
		//SetFloatArray(pDevice, "LightDir", *pSpot->GetDirection(), 3);  //(设备，shader中变量，某光照获取属性方法， float4 转为float3)

		//待定完成Spot属性

	}

	return S_OK;
}
