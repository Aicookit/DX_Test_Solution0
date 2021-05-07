#pragma once
#include "CDevice.h"
//简单测试平行光shader
class CShader
{
	LPDIRECT3DVERTEXSHADER9 LPshader;
	LPD3DXCONSTANTTABLE LPshaderConstantTable;

public:
	CShader();
	~CShader();

	void SetShader(LPDIRECT3DVERTEXSHADER9  m_pShader);

	void SetShaderTable(LPD3DXCONSTANTTABLE m_pShaderTable);

	LPDIRECT3DVERTEXSHADER9 GetShader();

	LPD3DXCONSTANTTABLE GetShaderTable();

public:
	//func  create shaderbuff and compile
	HRESULT CompileShaderAndCreatebuff(CDevice*pDevice, const char*pShaderName);
	HRESULT SetShader(CDevice*pDevice);
	HRESULT SetShaderValue(CDevice*pDevice, const char*pShaderVarName, D3DXMATRIX* SetpMatrixValue, D3DXVECTOR4*SetColor);
	HRESULT SetShaderMatrixValue(CDevice * pDevice, const char * pShaderVarName, D3DXMATRIX * SetpMatrixValue);
	HRESULT SetShaderLightValue(CDevice * pDevice, const char * pShaderVarName, D3DXVECTOR4 * SetColorValue);
	HRESULT SetShaderTexture(CDevice * pDevice, const char * pShaderVarName, D3DXVECTOR2 * SetTexutre);
};

