#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")  
#pragma comment(lib,"d3dx9.lib")  
#pragma comment(lib,"dxguid.lib")  
#pragma comment(lib, "winmm.lib")

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
bool Game_Init(HWND hwnd);
void Game_Update(float);
void Game_Render();
void Game_Exit();

LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;
LPD3DXMESH g_pSphere = NULL; //球体
LPD3DXMESH g_pCube = NULL; //立方体
//LPD3DXEFFECT g_pEffect = NULL; //效果框架--现在我们用VS/PS的常量表代替效果框架
LPDIRECT3DVERTEXSHADER9 m_pVShader;
LPDIRECT3DPIXELSHADER9 m_pPShader;
LPD3DXCONSTANTTABLE m_pVSConstTable;
LPD3DXCONSTANTTABLE	m_pPSconstTable;


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int iCmdShow)

{
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	static TCHAR szClassName[] = TEXT("WindowsApp");
	/* 第一步：注册窗口类 */
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szClassName;
	wndclass.lpfnWndProc = WindowProcedure;
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;

	if (!RegisterClass(&wndclass))
		return 0;

	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	/* 第二步：创建窗口 */
	hwnd = CreateWindow(
		szClassName,
		TEXT("MyApp"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	/* 第三步：显示窗口 */
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	Game_Init(hwnd);

	float lastTime = timeGetTime() * 0.001f;
	float currentTime, delta;
	/* 第四步：消息循环 */
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			currentTime = timeGetTime() * 0.001f;
			delta = currentTime - lastTime;
			lastTime = currentTime;
			Game_Update(delta);
			Game_Render();
		}
	}

	Game_Exit();

	return msg.wParam;
}

/*窗口过程*/
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}


//在初始化Game_Init()函数中使用着色器
HRESULT UseVShader(const char*pVShaderName)
{

	//编译-创建-释放-设默认VS/PS

	LPD3DXBUFFER shader = NULL;
	LPD3DXBUFFER errorBuffer = NULL;

	//1
	if (FAILED(D3DXCompileShaderFromFile(pVShaderName, 0, 0, "vs_main", "vs_2_0", D3DXSHADER_DEBUG, &shader, &errorBuffer, &m_pVSConstTable)))
	{
		if (errorBuffer)
		{
			const char * szp = (const char *)errorBuffer->GetBufferPointer();   // 报告错误顶点信息
			MessageBox(NULL, szp, "编译VSshader失败", MB_OK);
		}
		else
			MessageBox(NULL, "可能无法找到VShader文件", "编译VSshader失败", MB_OK);
		return E_FAIL;
	}

	//2
	if (FAILED(g_pd3dDevice->CreateVertexShader((DWORD*)shader->GetBufferPointer(), &m_pVShader)))
	{
		MessageBox(NULL, "创建VSshader失败", "创建VShader", MB_OK);
		return E_FAIL;
	}
	//3
	if (shader)
	{
		shader->Release();
	}
	if (errorBuffer)
	{
		errorBuffer->Release();
	}
	// 4
	m_pVSConstTable->SetDefaults(g_pd3dDevice);

	return S_OK;

}

//   PS
HRESULT UsePShader(const char*pPSshaderName) // 加载PS
{
	LPD3DXBUFFER shader = NULL;
	LPD3DXBUFFER errorBuffer = NULL;

	//编译PS
	if (FAILED(D3DXCompileShaderFromFile(pPSshaderName, 0, 0, "ps_main", "ps_2_0", D3DXSHADER_DEBUG, &shader, &errorBuffer, &m_pPSconstTable)))
	{
		if (errorBuffer)
		{
			const char * szp = (const char *)errorBuffer->GetBufferPointer();   // 报告错误顶点信息
			MessageBox(NULL, szp, "编译PSshader失败", MB_OK);
		}
		else
			MessageBox(NULL, "可能无法找到PShader文件", "编译PSshader失败", MB_OK);
		return E_FAIL;
	}

	//创建PS
	if (FAILED(g_pd3dDevice->CreatePixelShader((DWORD*)shader->GetBufferPointer(), &m_pPShader)))
	{
		MessageBox(NULL, "创建PS失败", "创建Pixel Shader", MB_OK);
		return E_FAIL;
	};

	//+4  对PShader中变量赋值前设为默认
	m_pPSconstTable->SetDefaults(g_pd3dDevice);

	if (shader)
	{
		shader->Release();
	}
	if (errorBuffer)
	{
		errorBuffer->Release();
	}
	return S_OK;
}

bool Game_Init(HWND hwnd)
{
	/* D3D初始化 */
	LPDIRECT3D9 d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 d3dcap;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcap);

	DWORD vp;
	if (d3dcap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	HRESULT hr;
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &g_pd3dDevice);

	d3d9->Release();

	if (FAILED(hr))
		return false;

	
	D3DXCreateSphere(g_pd3dDevice, 1.0f, 50, 50, &g_pSphere, NULL);//创建球体mesh, 球体半径为1.0
	D3DXCreateBox(g_pd3dDevice, 0.9f, 3.0f, 0.4f, &g_pCube, NULL);  // Box Mesh

	////编译-设默shader

	UseVShader("VSPointlightPhongs.txt");  // 编译-默认
	UsePShader("PSPointlightPhongs.txt");

	return true;
}

void Game_Update(float delta)
{
	/* 计算观察矩阵和投影矩阵，并传入Shader */
	D3DXMATRIX matView;
	D3DXVECTOR3 vEye(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 1000.0f);
	//把MVP转换替换到shader
	D3DXHANDLE hMatWVP = m_pVSConstTable->GetConstantByName(0, "MatWVP");
	m_pVSConstTable->SetMatrix(g_pd3dDevice, hMatWVP,&(matView * matProj));
	//将ViewPos传入Shader
	//D3DXHANDLE hViewPos = m_pVSConstTable->GetConstantByName(0, "ViewPos");
	//m_pVSConstTable->SetVector(g_pd3dDevice, hViewPos, &D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 1.0f));
	//要是视点位置是矩阵的话
	D3DXHANDLE hViewPos = m_pVSConstTable->GetConstantByName(0, "ViewPos");
	m_pVSConstTable->SetVector(g_pd3dDevice, hViewPos, &D3DXVECTOR4(vEye.x, vEye.y, vEye.z, 1.0f));

	D3DXHANDLE LightPosHandle = m_pVSConstTable->GetConstantByName(0, "LightPos");
	m_pVSConstTable->SetVector(g_pd3dDevice, LightPosHandle, &D3DXVECTOR4(0.0f, 3.0f, -3.0f,1.0f));
	D3DXHANDLE AttenuationHandle  = m_pVSConstTable->GetConstantByName(0, "Attenuation");
	m_pVSConstTable->SetVectorArray(g_pd3dDevice, AttenuationHandle,&D3DXVECTOR4(0.5f, 0.1f, 0.0f,1.0f), 4);
	//D3DXHANDLE AmbientLightHandle = m_pVSConstTable->GetConstantByName(0, "AmbientLight");       //----兰伯特点光vs ps
	//D3DXHANDLE DiffuseLightHandle = m_pVSConstTable->GetConstantByName(0, "DiffuseLight");
	//D3DXHANDLE SpecularLightHandle= m_pVSConstTable->GetConstantByName(0, "SpecularLight");
	//m_pVSConstTable->SetVector(g_pd3dDevice, AmbientLightHandle, &D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f));
	//m_pVSConstTable->SetVector(g_pd3dDevice, DiffuseLightHandle, &D3DXVECTOR4(0.1f, 0.1f, 1.0f, 1.0f));
	//m_pVSConstTable->SetVector(g_pd3dDevice, SpecularLightHandle, &D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));

	D3DXHANDLE AmbientLightHandle = m_pPSconstTable->GetConstantByName(0, "AmbientLight");       //----冯氏点光VS PS
	D3DXHANDLE DiffuseLightHandle = m_pPSconstTable->GetConstantByName(0, "DiffuseLight");
	D3DXHANDLE SpecularLightHandle= m_pPSconstTable->GetConstantByName(0, "SpecularLight");
	m_pPSconstTable->SetVector(g_pd3dDevice, AmbientLightHandle, &D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f));
	m_pPSconstTable->SetVector(g_pd3dDevice, DiffuseLightHandle, &D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f));
	m_pPSconstTable->SetVector(g_pd3dDevice, SpecularLightHandle, &D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f));

}

void Game_Render()
{
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(66, 0, 0), 1.0f, 0);
	g_pd3dDevice->BeginScene();

	/*g_pEffect->Begin(0, 0);
	g_pEffect->BeginPass(0);*/
	g_pd3dDevice->SetVertexShader(m_pVShader);  // 最后绑定shader到设备设置启动
	g_pd3dDevice->SetPixelShader(m_pPShader);

	g_pSphere->DrawSubset(0);
	//g_pCube->DrawSubset(0);

	/*g_pEffect->EndPass();
	g_pEffect->End();*/

	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void Game_Exit()
{
	SAFE_RELEASE(g_pCube);

	SAFE_RELEASE(g_pSphere);
	SAFE_RELEASE(m_pVShader);
	SAFE_RELEASE(m_pPShader);
	SAFE_RELEASE(m_pVSConstTable);
	SAFE_RELEASE(m_pPSconstTable);
		//SAFE_RELEASE(g_pEffect)
		SAFE_RELEASE(g_pd3dDevice);
}