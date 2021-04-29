#include "CDevice.h"

CDevice::CDevice()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pD3DDevice);
}

LPDIRECT3DDEVICE9 CDevice::GetD3DDevice()
{
	return m_pD3DDevice;
}

HRESULT CDevice::InitD3D(HWND hWnd)
{
	if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;        //添加深度缓存属性              //为什么打卡深度缓存无法渲染
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;   //深度缓存大小格式16位

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
		return E_FAIL;



	return S_OK;
}