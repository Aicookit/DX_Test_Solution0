//一层

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <windows.h>
#include "CDevice.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CModel.h"
#include<vector>
using namespace std;

CDevice * g_pDevice = NULL;
vector<CModel *> g_vecModels;

HRESULT InitRes()
{
	CModel * pModel;
	CMesh * pMesh;
	CMaterial * pMat;

	pModel = new CModel();
	pMesh = new CMesh();
	pMat = new CMaterial();

	pMesh->VertexDeclaration(g_pDevice);

	pMat->LoadTexture(g_pDevice, "Earth.jpg");
	pMesh->CreateSphere2(g_pDevice, 20, 20); //Sphere
	pModel->SetMaterial(pMat);
	pModel->SetMesh(pMesh);
	//pModel->SetPos(5.0f, 0.0f, 0.0f);
	g_vecModels.push_back(pModel);

	//pMesh->CreatePlane(g_pDevice, 3, 3, 3);

	return S_OK;
}

void CleanUp()
{
	for (size_t i = 0; i < g_vecModels.size(); i++)  // 执行渲染 模型对象
		delete g_vecModels[i];
	g_vecModels.clear();

	if (g_pDevice != NULL)
	{
		g_pDevice->GetD3DDevice()->Release();
	}

}

void Render()
{

	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &position, &target, &up);
	g_pDevice->GetD3DDevice()->SetTransform(D3DTS_VIEW, &view);//从CDevice类中拿到DX系统的方法添加视角转换

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 1000.0f);
	g_pDevice->GetD3DDevice()->SetTransform(D3DTS_PROJECTION, &proj);//从我的CDevice类中拿到DX的D3DDevice方法，进行投影转换

	D3DXMATRIX matWorld_X;
	D3DXMatrixIdentity(&matWorld_X);
	D3DXMatrixRotationX(&matWorld_X, GetTickCount() / 500.0f);
	D3DXMATRIX matWorld_Y;
	D3DXMatrixIdentity(&matWorld_Y);
	D3DXMatrixRotationY(&matWorld_Y, GetTickCount() / 500.0f);
	D3DXMATRIX matWorld_Z;
	D3DXMatrixIdentity(&matWorld_Z);
	D3DXMatrixRotationZ(&matWorld_Z, GetTickCount() / 500.0f);
	g_pDevice->GetD3DDevice()->SetTransform(D3DTS_WORLD, &(matWorld_X * matWorld_Y * matWorld_Z));

	g_pDevice->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 1);  // 调用CDevice 类来做

	if (SUCCEEDED(g_pDevice->GetD3DDevice()->BeginScene()))
	{

		for (size_t i = 0; i < g_vecModels.size(); i++)
			g_vecModels[i]->Render(g_pDevice);  // Render() include apply() and  RenderMesh()


		//SetRenderState为固定？
		g_pDevice->GetD3DDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); // 启动Z 缓存，允许消隐处理
		g_pDevice->GetD3DDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);  //关闭3剔除背面
		g_pDevice->GetD3DDevice()->SetRenderState(D3DRS_LIGHTING, false); //用材质灯光渲染就打开灯光||用顶点颜色渲染就关闭灯光

		g_pDevice->GetD3DDevice()->EndScene();
	}

	g_pDevice->GetD3DDevice()->Present(NULL, NULL, NULL, NULL);

}

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		CleanUp();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MsgProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, "");
	wc.hIconSm = LoadIcon(hInstance, "");
	wc.hCursor = LoadCursor(hInstance, "");
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Direct3D9 Example";

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindowEx(NULL,
		"Direct3D9 Example",
		"Direct3D9 Example",
		WS_OVERLAPPEDWINDOW,
		100, 100, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		wc.hInstance,
		NULL);

	//实例设备
	g_pDevice = new CDevice();
	if (SUCCEEDED(g_pDevice->InitD3D(hWnd)))     //作用？ 如果成功 InitD3D...设备
	{
		if (SUCCEEDED(InitRes()))               //如果成功 InitRes() 初始化加载的 Mesh Texture Mat Model Light Camera ...
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (true)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
				{
					if (msg.message == WM_QUIT)
						break;

					if (GetMessage(&msg, NULL, 0, 0))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else
					Render();
			}
		}
	}
	UnregisterClass("Direct3D10 Example", wc.hInstance);
	return 0;
}
//MSG msg 消息结构体
//HWND 类型的变量来标识窗口
//PeekMesssge是以查看的方式从系统中获取消息，可以不将消息从系统中移除，是非阻塞函数；当系统无消息时，返回FALSE，继续执行后续代码。
//PeekMessage通常不从队列里清除WM_PAINT消息。该消息将保留在队列里直到处理完毕。