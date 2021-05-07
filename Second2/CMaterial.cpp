#include "CMaterial.h"

CMaterial::CMaterial()
{
	m_pTexture = NULL;
}

CMaterial::~CMaterial()
{
	SAFE_RELEASE(m_pTexture);
	//pDevice->GetD3DDevice()->SetTexture(0, NULL);//���� ��Ⱦ��� �Ƴ�
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
		MessageBox(NULL, "�Ҳ�������ͼ", "D3DXCreateTextureFromFile()������ͼ�󶨵��豸", MB_OK);
		return E_FAIL;
	}
	return S_OK;

}
//Mat set texture, shader and bind to Device
HRESULT CMaterial::Apply(CDevice * pDevice, LPD3DXMATRIX lpWorldMatrix)
{
	pDevice->GetD3DDevice()->SetTexture(0, m_pTexture);  // 4 ʹ������  ���м�������ݲ�ʹ�����õ�4�۾ͻ�ɿ�ָ��

	return S_OK;
}
