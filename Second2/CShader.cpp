#include "CShader.h"

CShader::CShader()
{
	LPshader = NULL;
	LPshaderConstantTable = NULL;
}

CShader::~CShader()
{
	SAFE_RELEASE(LPshader);
	SAFE_RELEASE(LPshaderConstantTable);
}

void CShader:: SetShader(LPDIRECT3DVERTEXSHADER9 m_pShader)
{
	LPshader = m_pShader;
}

void CShader::SetShaderTable(LPD3DXCONSTANTTABLE m_pShaderTable)
{
	LPshaderConstantTable = m_pShaderTable;
}

LPDIRECT3DVERTEXSHADER9 CShader::GetShader()
{
	return LPshader;
}
LPD3DXCONSTANTTABLE CShader::GetShaderTable()
{
	return LPshaderConstantTable;
}


HRESULT CShader::CompileShaderAndCreatebuff(CDevice*pDevice, const char*pShaderName)
{

	LPD3DXBUFFER shaderBuff = NULL;
	LPD3DXBUFFER errorBuffer = NULL;

	//---4-1 编译
	if (FAILED(D3DXCompileShaderFromFile(pShaderName, 0, 0, "Main", "vs_2_0", D3DXSHADER_DEBUG, &shaderBuff, &errorBuffer, &LPshaderConstantTable)))
	{
		if (errorBuffer)
		{
			const char * szp = (const char *)errorBuffer->GetBufferPointer();   // 报告错误顶点信息
			MessageBox(NULL, szp, "编译Shader失败", MB_OK);
		}
		else
			MessageBox(NULL, "可能无法找到Shader文件", "编译Shader失败", MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->GetD3DDevice()->CreateVertexShader((DWORD*)shaderBuff->GetBufferPointer(), &LPshader)))
	{
		MessageBox(NULL, "创建shaderBuff失败", "创建shaderBuff", MB_OK);
		return E_FAIL;
	}

	if (shaderBuff)
	{
		shaderBuff->Release();
	}
	if (errorBuffer)
	{
		errorBuffer->Release();
	}

	LPshaderConstantTable->SetDefaults(pDevice->GetD3DDevice());

	return S_OK;
}
//设备别忘记启动shader

HRESULT CShader::SetShader(CDevice*pDevice)
{
	if (FAILED(pDevice->GetD3DDevice()->SetVertexShader(LPshader)))
	{
		MessageBox(NULL, "启动Vshader失败", "启动shader", MB_OK);
		return E_FAIL;
	}
	return S_OK;

}

HRESULT CShader::SetShaderValue(CDevice*pDevice, const char*pShaderVarName, D3DXMATRIX* SetpMatrixValue, D3DXVECTOR4*SetColor)  // shader中变量类型有很多,这里只能设Matrix
{
	D3DXHANDLE MatrixHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);
	D3DXHANDLE ColorHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);

	if (MatrixHandle == NULL)
	{
		MessageBox(NULL, "MatrixHandle is Empty", "获取shadeMatrix变量", MB_OK);
		return E_FAIL;
	}
	if (ColorHandle==NULL)
	{
		MessageBox(NULL, "ColorHandle is Empty", "获取shadeColor变量", MB_OK);
		return E_FAIL;
	}

	LPshaderConstantTable->SetMatrix(pDevice->GetD3DDevice(), MatrixHandle, SetpMatrixValue); //这里SetMatrix()只能对Matrix设值
	LPshaderConstantTable->SetVector(pDevice->GetD3DDevice(), ColorHandle, SetColor); //这里SetMatrix()只能对Matrix设值

	return S_OK;
}

HRESULT CShader::SetShaderMatrixValue(CDevice*pDevice, const char*pShaderVarName, D3DXMATRIX* SetpMatrixValue)  // shader中变量类型有很多,这里只能设Matrix
{
	D3DXHANDLE MatrixHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);


	if (MatrixHandle == NULL)
	{
		MessageBox(NULL, "MatrixHandle is Empty", "获取shadeMatrix变量", MB_OK);
		return E_FAIL;
	}
	LPshaderConstantTable->SetMatrix(pDevice->GetD3DDevice(), MatrixHandle, SetpMatrixValue); 

	return S_OK;

}
HRESULT CShader::SetShaderLightValue(CDevice*pDevice, const char*pShaderVarName, D3DXVECTOR4* SetColorValue)  // shader中变量类型有很多,这里只能设Matrix
{
	D3DXHANDLE LightHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);


	if (LightHandle == NULL)
	{
		MessageBox(NULL, "LightHandle is Empty", "获取Color变量", MB_OK);
		return E_FAIL;
	}
	LPshaderConstantTable->SetVector(pDevice->GetD3DDevice(), LightHandle, SetColorValue); //这里SetMatrix()只能对Matrix设值

	return S_OK;

}
HRESULT CShader::SetShaderTexture(CDevice*pDevice, const char *pShaderVarName, D3DXVECTOR2*SetTexutre)
{
	//D3DXHANDLE ColorHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);


	//if (ColorHandle == NULL)
	//{
	//	MessageBox(NULL, "MatrixHandle is Empty", "获取shadeMatrix变量", MB_OK);
	//	return E_FAIL;
	//}
	//LPshaderConstantTable->SetVector(pDevice->GetD3DDevice(), ColorHandle, SetColorValue); //这里SetMatrix()只能对Matrix设值

	return S_OK;
}
