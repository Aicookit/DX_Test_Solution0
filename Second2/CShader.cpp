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

	//---4-1 ����
	if (FAILED(D3DXCompileShaderFromFile(pShaderName, 0, 0, "Main", "vs_2_0", D3DXSHADER_DEBUG, &shaderBuff, &errorBuffer, &LPshaderConstantTable)))
	{
		if (errorBuffer)
		{
			const char * szp = (const char *)errorBuffer->GetBufferPointer();   // ������󶥵���Ϣ
			MessageBox(NULL, szp, "����Shaderʧ��", MB_OK);
		}
		else
			MessageBox(NULL, "�����޷��ҵ�Shader�ļ�", "����Shaderʧ��", MB_OK);
		return E_FAIL;
	}

	if (FAILED(pDevice->GetD3DDevice()->CreateVertexShader((DWORD*)shaderBuff->GetBufferPointer(), &LPshader)))
	{
		MessageBox(NULL, "����shaderBuffʧ��", "����shaderBuff", MB_OK);
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
//�豸����������shader

HRESULT CShader::SetShader(CDevice*pDevice)
{
	if (FAILED(pDevice->GetD3DDevice()->SetVertexShader(LPshader)))
	{
		MessageBox(NULL, "����Vshaderʧ��", "����shader", MB_OK);
		return E_FAIL;
	}
	return S_OK;

}

HRESULT CShader::SetShaderValue(CDevice*pDevice, const char*pShaderVarName, D3DXMATRIX* SetpMatrixValue, D3DXVECTOR4*SetColor)  // shader�б��������кܶ�,����ֻ����Matrix
{
	D3DXHANDLE MatrixHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);
	D3DXHANDLE ColorHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);

	if (MatrixHandle == NULL)
	{
		MessageBox(NULL, "MatrixHandle is Empty", "��ȡshadeMatrix����", MB_OK);
		return E_FAIL;
	}
	if (ColorHandle==NULL)
	{
		MessageBox(NULL, "ColorHandle is Empty", "��ȡshadeColor����", MB_OK);
		return E_FAIL;
	}

	LPshaderConstantTable->SetMatrix(pDevice->GetD3DDevice(), MatrixHandle, SetpMatrixValue); //����SetMatrix()ֻ�ܶ�Matrix��ֵ
	LPshaderConstantTable->SetVector(pDevice->GetD3DDevice(), ColorHandle, SetColor); //����SetMatrix()ֻ�ܶ�Matrix��ֵ

	return S_OK;
}

HRESULT CShader::SetShaderMatrixValue(CDevice*pDevice, const char*pShaderVarName, D3DXMATRIX* SetpMatrixValue)  // shader�б��������кܶ�,����ֻ����Matrix
{
	D3DXHANDLE MatrixHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);


	if (MatrixHandle == NULL)
	{
		MessageBox(NULL, "MatrixHandle is Empty", "��ȡshadeMatrix����", MB_OK);
		return E_FAIL;
	}
	LPshaderConstantTable->SetMatrix(pDevice->GetD3DDevice(), MatrixHandle, SetpMatrixValue); 

	return S_OK;

}
HRESULT CShader::SetShaderLightValue(CDevice*pDevice, const char*pShaderVarName, D3DXVECTOR4* SetColorValue)  // shader�б��������кܶ�,����ֻ����Matrix
{
	D3DXHANDLE LightHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);


	if (LightHandle == NULL)
	{
		MessageBox(NULL, "LightHandle is Empty", "��ȡColor����", MB_OK);
		return E_FAIL;
	}
	LPshaderConstantTable->SetVector(pDevice->GetD3DDevice(), LightHandle, SetColorValue); //����SetMatrix()ֻ�ܶ�Matrix��ֵ

	return S_OK;

}
HRESULT CShader::SetShaderTexture(CDevice*pDevice, const char *pShaderVarName, D3DXVECTOR2*SetTexutre)
{
	//D3DXHANDLE ColorHandle = LPshaderConstantTable->GetConstantByName(0, pShaderVarName);


	//if (ColorHandle == NULL)
	//{
	//	MessageBox(NULL, "MatrixHandle is Empty", "��ȡshadeMatrix����", MB_OK);
	//	return E_FAIL;
	//}
	//LPshaderConstantTable->SetVector(pDevice->GetD3DDevice(), ColorHandle, SetColorValue); //����SetMatrix()ֻ�ܶ�Matrix��ֵ

	return S_OK;
}
