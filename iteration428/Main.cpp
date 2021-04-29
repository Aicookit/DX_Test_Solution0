//一层

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <windows.h>
#include "CDevice.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CModel.h"
#include"CLight.h"
#include"CCamera.h"
#include<vector>


CDevice * g_pDevice = NULL;
CLight  * g_pLight  =  NULL;
CCamera * g_pCamera = NULL;
std::vector<CModel *> g_vecModels;

HRESULT InitRes()
{
	CLightDirectional *pLightDir = new CLightDirectional();  // 1 建立数据
	pLightDir->SetAmbientMat(0.0f, 0.0f, 1.0f, 1.0f);       // 2 设置数据
	pLightDir->SetDiffuseMat(0.0f, 0.0f, 1.0f, 1.0f);
	//pLightDire->SetSpecularMat(0.0f, 0.0f, 1.0f, 1.0f);
	pLightDir->SetDirection(-0.57f, 0.57f, -0.57f);
	g_pLight = pLightDir;

	g_pCamera = new CCamera();
	g_pCamera->SetPositon(0.0f, 0.0f, -5.0f);  // set View
	g_pCamera->SetTarget(0.0f, 0.0f, 0.0f);
	g_pCamera->SetUp(0.0f, 1.0f, 0.0f);

	CModel * pModel;
	CMesh * pMesh;
	CMaterial * pMat;

	pModel = new CModel();
	pMesh = new CMesh();
	pMat = new CMaterial();

	pMesh->VertexDeclaration(g_pDevice);

	pMat->LoadTexture(g_pDevice, "Cat.jpg");  // mat load texture
	pMat->LoadVShader(g_pDevice, "VSLightDirection.txt"); // mat load shader
	pMat->LoadPShader(g_pDevice, "PSLightDirection.txt"); // mat load shader

	pMesh->CreateSphere2(g_pDevice, 20, 20); //Sphere-Mesh
	//pMesh->CreateTriangle(g_pDevice);   //Triangle-Mesh
	//pMesh->CreatePlane(g_pDevice, 4, 4, 4); // Plane-Mesh

	pModel->SetMaterial(pMat);
	pModel->SetMesh(pMesh);
	//pModel->SetPos(5.0f, 0.0f, 0.0f);
	g_vecModels.push_back(pModel);

	return S_OK;
}

void CleanUp()
{
	if (g_pLight!=NULL)
	{
		delete g_pLight;
		g_pLight = NULL;
	}

	if (g_pCamera!=NULL)
	{
		delete g_pCamera;
		g_pCamera = NULL;
	}

	for (size_t st = 0; st < g_vecModels.size(); ++st)  // 执行渲染 模型对象
		delete g_vecModels[st];

	g_vecModels.clear();

	if (g_pDevice != NULL)
	{
		g_pDevice->GetD3DDevice()->Release();
	}
}
/*
void SetValue()
{
	CLightDirectional *pLightDir = new CLightDirectional();
	pLightDir->SetAmbientMat(0.0f, 0.0f, 1.0f, 1.0f);
	pLightDir->SetDiffuseMat(0.0f, 0.0f, 1.0f, 1.0f);
	//pLightDire->SetSpecularMat(0.0f, 0.0f, 1.0f, 1.0f);
	pLightDir->SetDirection(-0.57f, 0.57f, -0.57f);
	//g_pLight = pLightDir;                //  assign to global
	g_pCamera = new CCamera();
	g_pCamera->SetPositon(0.0f, 0.0f, -5.0f);  // set View
	g_pCamera->SetTarget(0.0f, 0.0f, 0.0f);
	g_pCamera->SetUp(0.0f, 1.0f, 0.0f);
	
	g_pCamera->SetProjMatrix(4.0f, 1.0f, 1000.0f);  //set Proj
	
	//对ViewProj矩阵的设置用 Mat->SetMatrix()调？
	g_pCamera->SetVPMatrix(g_pDevice, *g_pCamera->GetViewMatrix(), *g_pCamera->GetProjMatrix());  // set VP
}*/

void Render()
{
	// assign constant value to variable in shader for update in WND

	//SetValue();

	g_pCamera->Update();  //4 使用3更新数据

	g_pDevice->GetD3DDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 1);  // 调用CDevice 类来做

	if (SUCCEEDED(g_pDevice->GetD3DDevice()->BeginScene()))
	{
		for (size_t st = 0; st < g_vecModels.size(); ++st)
			g_vecModels[st]->Render(g_pDevice, g_pLight, g_pCamera);  // Render() include apply() and  RenderMesh()

		//SetRenderState为固定？
		//g_pDevice->GetD3DDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE); // 启动Z 缓存，允许消隐处理
		//g_pDevice->GetD3DDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);  //关闭3剔除背面
		//g_pDevice->GetD3DDevice()->SetRenderState(D3DRS_LIGHTING, false); //用材质灯光渲染就打开灯光||用顶点颜色渲染就关闭灯光

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