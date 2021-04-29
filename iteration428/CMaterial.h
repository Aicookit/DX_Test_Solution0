#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "CDevice.h"
#include "CCamera.h"
#include "CLight.h"

class CCamera;

//CMaterial organize texture and  Shader

class  CMaterial
{
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXSHADER9 m_pVShader;
	LPDIRECT3DPIXELSHADER9 m_pPShader;
	LPD3DXCONSTANTTABLE m_pVSConstTable;
	LPD3DXCONSTANTTABLE	m_pPSconstTable;

public:
	 CMaterial();
	~ CMaterial();

public:
	LPDIRECT3DTEXTURE9 GetTexture();  //获取当前贴图

	LPDIRECT3DVERTEXSHADER9 GetVShader();
	LPDIRECT3DPIXELSHADER9 GetPShader();
	
	HRESULT LoadTexture(CDevice * pDevice, const char* pTextureName); //加载该贴图到缓存

	HRESULT LoadVShader(CDevice * pDevice, const char * pVShaderName);

	HRESULT LoadPShader(CDevice * pDevice, const char * pPSshaderName);

	HRESULT SetMatrix(CDevice * pDevice, const char * pHandle, D3DXMATRIX * mMatrix);

	HRESULT SetVector(CDevice * pDevice, const char * pHandle, D3DXVECTOR4 * fVector);

	HRESULT SetFloat(CDevice * pDevice, const char * pHandle, FLOAT fFloat);

	HRESULT SetFloatArray(CDevice*pDevice, const char*pHandle, FLOAT *  pFloat, int iCount);

	HRESULT Apply(CDevice * pDevice,  LPD3DXMATRIX lpWorld,CLight * pLight, CCamera * pCamera);//应用包括 贴图、光照shader。。

};

