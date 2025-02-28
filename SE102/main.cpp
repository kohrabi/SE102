/* =============================================================
INTRODUCTION TO GAME PROGRAMMING SE102

SAMPLE 00 - INTRODUCTORY CODE

This sample illustrates how to:

1/ Create a window
2/ Initiate Direct3D 10, DirectX Sprite
3/ Draw a static brick sprite to the screen
4/ Create frame rate independent movements

5/ Some good C++ programming practices
- Use constants whenever possible
- 0 Warnings

6/ Debug using __FILE__ __LINE__

HOW TO INSTALL Microsoft.DXSDK.D3DX
===================================
1) Tools > NuGet package manager > Package Manager Console
2) execute command :  Install-Package Microsoft.DXSDK.D3DX


WARNING: This one file example has a hell LOT of *sinful* programming practices
================================================================ */
#include <windows.h>

#include <d3d10.h>
#include <d3dx10.h>

#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>

#include <comdef.h>

#define WINDOW_CLASS_NAME L"SampleWindow"
#define WINDOW_TITLE L"00 - Intro"
#define WINDOW_ICON_PATH L"brick.ico" 

HWND hWnd = 0;

// Each color is from 0.0f to 1.0f  ( 0/255 to 255/255 ) 
#define BACKGROUND_COLOR D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f)

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define MAX_FRAME_RATE 100

ID3D10Device* pD3DDevice = NULL;
IDXGISwapChain* pSwapChain = NULL;
ID3D10RenderTargetView* pRenderTargetView = NULL;

int BackBufferWidth = 0;
int BackBufferHeight = 0;

#define TEXTURE_PATH_POOL L"pool.png"
#define POOL_START_X WINDOW_WIDTH / 2.0f
#define POOL_START_Y WINDOW_HEIGHT / 2.0f

#define POOL_START_VEL 0.2f

#define POOL_WIDTH 84.0f / 3.0f
#define POOL_HEIGHT POOL_WIDTH
#define MAX_POOL 10
#define POOL_FRICTION 0.98f

ID3D10Texture2D* texPool = NULL;				// Texture object to store brick image
ID3DX10Sprite* spriteObject = NULL;				// Sprite handling object 

D3DX10_SPRITE spritePool;

float pool_x[MAX_POOL] = { POOL_START_X };
float pool_y[MAX_POOL] = { POOL_START_Y };
float pool_vx[MAX_POOL] = { 0.0 };
float pool_vy[MAX_POOL] = { 0.0 };
bool canFire = true;


bool keyState[256] = { false };
bool leftMouseDown = false;

POINT GetMousePosition(HWND hwnd)
{
	POINT mousePos;
	GetCursorPos(&mousePos);          // Get mouse position in screen coordinates
	ScreenToClient(hwnd, &mousePos);  // Convert to client area coordinates
	return mousePos;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYUP:
		keyState[wParam] = false;
		break;
	case WM_KEYDOWN:
		keyState[wParam] = true;
		break;
	case WM_LBUTTONDOWN:
		leftMouseDown = true;
		break;
	case WM_LBUTTONUP:
		leftMouseDown = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// DEBUG SUPPORT FUNCTIONS //////////////
#define _W(x)  __W(x)
#define __W(x)  L##x

#define VA_PRINTS(s) {				\
		va_list argp;				\
		va_start(argp, fmt);		\
		vswprintf_s(s, fmt, argp);	\
		va_end(argp);				\
}		

void DebugOut(const wchar_t* fmt, ...)
{
	wchar_t s[4096];
	VA_PRINTS(s);
	OutputDebugString(s);
}

void DebugOutTitle(const wchar_t* fmt, ...)
{
	wchar_t s[1024];
	VA_PRINTS(s);
	SetWindowText(hWnd, s);
}
//////////////////////////////////////////

void InitDirectX(HWND hWnd)
{

	// retrieve client area width & height so that we can create backbuffer height & width accordingly 
	RECT r;
	GetClientRect(hWnd, &r);

	BackBufferWidth = r.right + 1;
	BackBufferHeight = r.bottom + 1;


	// Create & clear the DXGI_SWAP_CHAIN_DESC structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill in the needed values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = BackBufferWidth;
	swapChainDesc.BufferDesc.Height = BackBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	HRESULT hr = S_OK;

	//
	// Uncomment the following section to query graphic cards on the computer
	//  

	/*
	IDXGIFactory* pFactory = NULL;
	#pragma comment(lib, "dxgi")
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (hr != S_OK)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();

		DebugOut((wchar_t*)L"[ERROR] CreateDXGIFactory has failed %s %d %d %s\n", _W(__FILE__), __LINE__, hr, errMsg);
		return;
	}

	IDXGIAdapter* pAdapter = NULL;
	for (UINT i = 0;
			pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
	 ++i)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		pAdapter->GetDesc(&adapterDesc);
		DebugOut((wchar_t*)L"[INFO] Adapter %d: %s\n",i, adapterDesc.Description);
	}
	*/

	// Create the D3D device and the swap chain
	hr = D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pD3DDevice);

	if (hr != S_OK)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();

		DebugOut((wchar_t*)L"[ERROR] D3D10CreateDeviceAndSwapChain has failed %s %d %d %s\n", _W(__FILE__), __LINE__, hr, errMsg);
		return;
	}

	// Get the back buffer from the swapchain
	ID3D10Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] pSwapChain->GetBuffer has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// create the render target view
	hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	// release the back buffer
	pBackBuffer->Release();

	// Make sure the render target view was created successfully
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] CreateRenderTargetView has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// set the render target
	pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// create and set the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = BackBufferWidth;
	viewPort.Height = BackBufferHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pD3DDevice->RSSetViewports(1, &viewPort);


	// create the sprite object to handle sprite drawing 
	hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteObject);

	// Make sure the sprite creation was successful
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3DX10CreateSprite has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	D3DXMATRIX matProjection;

	// Create the projection matrix using the values in the viewport
	D3DXMatrixOrthoOffCenterLH(&matProjection,
		(float)viewPort.TopLeftX,
		(float)viewPort.Width,
		(float)viewPort.TopLeftY,
		(float)viewPort.Height,
		0.1f,
		10);
	hr = spriteObject->SetProjectionTransform(&matProjection);

	ID3D10BlendState* g_pBlendState = NULL;
	D3D10_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D10_BLEND_DESC));

	BlendState.BlendEnable[0] = TRUE;
	BlendState.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	BlendState.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	BlendState.BlendOp = D3D10_BLEND_OP_ADD;
	BlendState.SrcBlendAlpha = D3D10_BLEND_ZERO;
	BlendState.DestBlendAlpha = D3D10_BLEND_ZERO;
	BlendState.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	BlendState.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	pD3DDevice->CreateBlendState(&BlendState, &g_pBlendState);
	pD3DDevice->OMSetBlendState(g_pBlendState, 0, 0xffffffff);


	DebugOut((wchar_t*)L"[INFO] InitDirectX has been successful\n");

	return;
}

float random_float(float min, float max) { return ((float)rand() / RAND_MAX) * (max - min) + min; }
/*
	Load game resources. In this example, we only load a brick image
*/
void LoadResources()
{
	ID3D10Resource* pD3D10Resource = NULL;

	// Loads the texture into a temporary ID3D10Resource object
	HRESULT hr = D3DX10CreateTextureFromFile(pD3DDevice,
		TEXTURE_PATH_POOL,
		NULL,
		NULL,
		&pD3D10Resource,
		NULL);

	// Make sure the texture was loaded successfully
	if (FAILED(hr))
	{
		DebugOut((wchar_t*)L"[ERROR] Failed to load texture file: %s \n", TEXTURE_PATH_POOL);
		return;
	}

	// Translates the ID3D10Resource object into a ID3D10Texture2D object
	pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&texPool);
	pD3D10Resource->Release();

	if (!texPool) {
		DebugOut((wchar_t*)L"[ERROR] Failed to convert from ID3D10Resource to ID3D10Texture2D \n");
		return;
	}

	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	texPool->GetDesc(&desc);

	// Create a shader resource view of the texture
	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;

	// Clear out the shader resource view description structure
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));

	// Set the texture format
	SRVDesc.Format = desc.Format;
	// Set the type of resource
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D10ShaderResourceView* gSpriteTextureRV = NULL;

	pD3DDevice->CreateShaderResourceView(texPool, &SRVDesc, &gSpriteTextureRV);

	// Set the sprite�s shader resource view
	spritePool.pTexture = gSpriteTextureRV;

	// top-left location in U,V coords
	spritePool.TexCoord.x = 0;
	spritePool.TexCoord.y = 0;

	// Determine the texture size in U,V coords
	spritePool.TexSize.x = 1.0f;
	spritePool.TexSize.y = 1.0f;

	// Set the texture index. Single textures will use 0
	spritePool.TextureIndex = 0;

	// The color to apply to this sprite, full color applies white.
	spritePool.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	srand(time(NULL));
	for (int i = 0; i < MAX_POOL; i++) {
		pool_x[i] = BackBufferWidth / 2.0f + (float(i) - MAX_POOL / 2.0f - 1) * POOL_WIDTH;
		pool_y[i] = BackBufferHeight / 2.0f + (float(i) - MAX_POOL / 2.0f - 1) * POOL_HEIGHT;
		pool_vx[i] = POOL_START_VEL * random_float(-0.5f, 1.0f);
		pool_vy[i] = POOL_START_VEL * random_float(-0.5f, 1.0f);
	}

	DebugOut((wchar_t*)L"[INFO] Texture loaded Ok: %s \n", TEXTURE_PATH_POOL);
}
/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame

	IMPORTANT: no render-related code should be used inside this function.
*/
bool choosing = false;
float hitPool_x = -1;
float hitPool_y = -1;
int hitPool = -1;

bool checkCollide(float x1, float y2, float x, float y, float radius) {
	return (x - x1) * (x - x1) + (y - y2) * (y - y2) <= radius * radius;
}


float pointLength(float x, float y) {
	return sqrt(x * x + y * y);
}

void normalizePoint(float& x, float& y) {
	float distance = pointLength(x, y);
	if (distance <= 0.001) return;
	x /= distance;
	y /= distance;
}

void Update(DWORD dt)
{
	//Uncomment the whole function to see the brick moves and bounces back when hitting left and right edges
	//brick_x++;

	POINT mousePos = GetMousePosition(hWnd);
	mousePos.y = BackBufferHeight - mousePos.y;
	if (leftMouseDown) {
		if (!choosing) {
			choosing = true;
			for (int i = 0; i < MAX_POOL; i++) {
				if (checkCollide(mousePos.x, mousePos.y, pool_x[i], pool_y[i], POOL_WIDTH)) {
					hitPool_x = pool_x[i];
					hitPool_y = pool_y[i];
					hitPool = i;
					break;
				}
			}
		}
	}
	else { // Left mouse up
		if (hitPool != -1) {
			pool_vx[hitPool] = hitPool_x - mousePos.x;
			pool_vy[hitPool] = hitPool_y - mousePos.y;
			float distance = pointLength(pool_vx[hitPool], pool_vy[hitPool]);
			pool_vx[hitPool] /= 100;
			pool_vy[hitPool] /= 100;
			pool_vx[hitPool] *= 1;
			pool_vy[hitPool] *= 1;
		}
		hitPool = -1;
		hitPool_x = -1;
		hitPool_y = -1;
		choosing = false;
	}
	
	for (int i = 0; i < MAX_POOL; i++) {
		for (int j = 0; j < MAX_POOL; j++) {
			if (i == j) continue;
			if (checkCollide(pool_x[i], pool_y[i], pool_x[j], pool_y[j], POOL_WIDTH)) {

				float direction_x = 1.f * (pool_x[j] - pool_x[i]);
				float direction_y = 1.0f * (pool_y[j] - pool_y[i]);
				normalizePoint(direction_x, direction_y);
				float leng = pointLength(pool_vx[i], pool_vy[i]);
				DebugOut(L"Collide: %d %d \n", i, j);
				DebugOut(L"%d %f \n", i, leng);
				pool_vx[j] += leng * direction_x;
				pool_vy[j] += leng * direction_y;
				pool_vx[i] *= 0.8f;
				pool_vy[i] *= 0.8f;
				pool_vx[j] *= 0.8f;
				pool_vy[j] *= 0.8f;
			}
		}
	}

	for (int i = 0; i < MAX_POOL; i++) {

		//if (keyState[VK_UP] && canFire) {
		//	pool_x = POOL_START_X;
		//	pool_y = POOL_START_Y;
		//	pool_vy = POOL_START_VEL;
		//	canFire = false;
		//}
		//DebugOut(L"MousePosition: %d %d\n", mousePos.x, mousePos.y);
		pool_x[i] += pool_vx[i] * dt;
		pool_y[i] += pool_vy[i] * dt;
		pool_vx[i] *= POOL_FRICTION;
		pool_vy[i] *= POOL_FRICTION;

		// NOTE: BackBufferWidth is indeed related to rendering!!
		float right_edge = BackBufferWidth - POOL_WIDTH;
		float bottom_edge = BackBufferHeight - POOL_HEIGHT;


		if (pool_x[i] < 0 || pool_x[i] > right_edge) {

			pool_vx[i] = -pool_vx[i];

			////Why not having these logics would make the brick disappear sometimes?  
			if (pool_x[i] < 0)
			{
				pool_x[i] = 0;
			}
			else if (pool_x[i] > right_edge )
			{
				pool_x[i] = right_edge;
			}
		}
		if (pool_y[i] < 0 || pool_y[i] > bottom_edge) {

			pool_vy[i] = -pool_vy[i];

			//Why not having these logics would make the brick disappear sometimes?  
			if (pool_y[i] < 0)
			{
				pool_y[i] = 0;
			}
			else if (pool_y[i] > bottom_edge)
			{
				pool_y[i] = bottom_edge;
			}
		}
	}
}

/*
	Render a frame
	IMPORTANT: world status must NOT be changed during rendering
*/
void Render()
{
	if (pD3DDevice != NULL)
	{
		// clear the target buffer
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		// start drawing the sprites
		spriteObject->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		for (int i = 0; i < MAX_POOL; i++) {

			// The translation matrix to be created
			D3DXMATRIX matTranslation;
			// Create the translation matrix
			D3DXMatrixTranslation(&matTranslation, pool_x[i], pool_y[i], 0.1f);

			// Scale the sprite to its correct width and height
			D3DXMATRIX matScaling;
			float multiplier = 1.0f;
			if (i == hitPool)
				multiplier = 1.2f;
			D3DXMatrixScaling(&matScaling, POOL_WIDTH * multiplier, POOL_HEIGHT * multiplier, 1.0f);

			// Setting the sprite position and size
			spritePool.matWorld = (matScaling * matTranslation);

			spriteObject->DrawSpritesImmediate(&spritePool, 1, 0, 0);

		}
		// Finish up and send the sprites to the hardware
		spriteObject->End();

		//DebugOutTitle((wchar_t*)L"%s (%0.1f,%0.1f) v:%0.1f", WINDOW_TITLE, brick_x, brick_y, brick_vx);

		// display the next item in the swap chain
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	//Try this to see how the debug function prints out file and line 
	//wc.hInstance = (HINSTANCE)-100; 

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut((wchar_t*)L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;
	OutputDebugStringW(L"My output string.");

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();
		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}

	return 1;
}

void Cleanup()
{
	// release the rendertarget
	if (pRenderTargetView)
	{
		pRenderTargetView->Release();
	}
	// release the swapchain
	if (pSwapChain)
	{
		pSwapChain->Release();
	}
	// release the D3D Device
	if (pD3DDevice)
	{
		pD3DDevice->Release();
	}

	if (spriteObject)
	{
		spriteObject->Release();
		spriteObject = NULL;
	}

	DebugOut((wchar_t*)L"[INFO] Cleanup Ok\n");
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	hWnd = CreateGameWindow(hInstance, nCmdShow, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (hWnd == 0) return 0;

	//AllocConsole();
	//freopen_s("CONIN$", "r", stdin);
	//freopen_s("CONOUT$", "w", stdout);
	//freopen_s("CONOUT$", "w", stderr);

	InitDirectX(hWnd);

	LoadResources();
	Run();
	Cleanup();

	return 0;
}