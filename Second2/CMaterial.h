#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "CDevice.h"

//CMaterial organize texture and  Shader

class  CMaterial
{
public:
	 CMaterial();
	~ CMaterial();

private:
	LPDIRECT3DTEXTURE9 m_pTexture;

	LPDIRECT3DVERTEXSHADER9 m_pVShader;
	LPD3DXCONSTANTTABLE m_pVSConstTable;

public:
	LPDIRECT3DTEXTURE9 GetTexture();  //获取当前贴图
	
	HRESULT LoadTexture(CDevice * pDevice, const char* pTextureName); //加载该贴图到缓存

	HRESULT LoadVS(CDevice * pDevice, const char * pShaderName);
	HRESULT SetVSHandle(CDevice*pDevice, const char*pHandle, D3DXMATRIX * mMatrix, D3DXVECTOR4 * fVector);

	HRESULT Apply(CDevice * pDevice, LPD3DXMATRIX lpWorldMatrix);//应用包括 贴图、光照shader。。

};

