#include "CMaterial.h"

CMaterial::CMaterial()
{
	m_pTexture = NULL;
}

CMaterial::~CMaterial()
{
	SAFE_RELEASE(m_pTexture);
	//pDevice->GetD3DDevice()->SetTexture(0, NULL);//纹理 渲染完后 移除
}

LPDIRECT3DTEXTURE9 CMaterial::GetTexture()
{
	return m_pTexture;
}
//Mat load texture function
HRESULT CMaterial::LoadTexture(CDevice * pDevice, const char *pTextureName)
{
	if (FAILED(D3DXCreateTextureFromFile(pDevice->GetD3DDevice(), pTextureName, &m_pTexture)))
	{
		MessageBox(NULL, "找不到该贴图", "D3DXCreateTextureFromFile()加载贴图绑定到设备", MB_OK);
		return E_FAIL;
	}
	return S_OK;

}
//Mat set texture, shader and bind to Device
HRESULT CMaterial::Apply(CDevice * pDevice)
{
	pDevice->GetD3DDevice()->SetTexture(0, m_pTexture);
	return S_OK;
}
