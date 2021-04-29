#pragma once  //¶þ²ã

#include <d3d9.h>
#include <d3dx9.h>

#define WINDOW_WIDTH			800
#define WINDOW_HEIGHT			600
#define SAFE_RELEASE(p)			{ if ((p) != NULL) { (p)->Release(); (p) = NULL; } }

class CDevice
{
	LPDIRECT3D9 m_pD3D ;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

public:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9 GetD3DDevice();

	HRESULT InitD3D(HWND hWnd);
};