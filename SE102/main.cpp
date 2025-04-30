/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 01 - SKELETON CODE 

	This sample illustrates how to:

	1/ Re-organize introductory code to an initial skeleton for better scalability
	2/ CGame is a singleton, playing a role of an "engine".
	3/ CGameObject is an abstract class for all game objects
	4/ CTexture is a wrapper class for ID3D10TEXTURE 
	
	NOTE: to create transparent background, download GIMP, then use Color to Alpha feature 
================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <vector>
#include <typeinfo>

#include "Engine/debug.h"
#include "Engine/Game.h"
#include "Engine/GameObject.h"
#include "contents.h"
#include "Engine/Graphics/Textures.h"
#include "Engine/Graphics/Sprites.h"
#include "Engine/Graphics/Animations.h"
#include "Engine/Loaders/SpritesLoader.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <iostream>
#include <Engine/Helper.h>

#define WINDOW_CLASS_NAME L"Game Window"
#define MAIN_WINDOW_TITLE L"MARIO"
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 208 + 30 + 28


using namespace std;

tmx::Map tMap;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		CGame::GetInstance()->SetKeyState((int)wParam, true);
		break;
	case WM_KEYUP:
		CGame::GetInstance()->SetKeyState((int)wParam, false);
		break;
	case WM_LBUTTONDOWN:
		CGame::GetInstance()->leftMouseDown = true;
		break;
	case WM_LBUTTONUP:
		CGame::GetInstance()->leftMouseDown = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


/*
	Load all game resources. In this example, create a brick object and mario object
*/
void LoadResources()
{
	CGame* const game = CGame::GetInstance();
	CTextures* const textures = CTextures::GetInstance();
	textures->Add(TEXTURE_PATH_BBOX);

	game->Load(SCENES_LIST_PATH);
	
	int backBufferWidth = game->GetBackBufferWidth();
	int backBufferHeight = game->GetBackBufferHeight();
	game->SetCamPos(0, 100);
}

void UnloadResources() {
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(float dt)
{
	CGame* const game = CGame::GetInstance();

	game->GetCurrentScene()->Update(dt);

}

/*
	Render a frame 
*/
void Render()
{
	CGame* const g = CGame::GetInstance();


	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	// clear the background 
	pD3DDevice->ClearRenderTargetView(pRenderTargetView, g->GetCurrentScene()->GetBackgroundColor());
	
	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);
	
	// Use Alpha blending for transparent sprites
	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);
	g->SetPointSamplerState();

	CGame::GetInstance()->GetCurrentScene()->Render();

	spriteHandler->End();
	pSwapChain->Present(0, 0);

	g->SetPrevKeyState();
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, NULL, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
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
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	constexpr ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;
	CGame* const game = CGame::GetInstance();

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
		ULONGLONG unscaledDt = (now - frameStart);
		float dt = (float)unscaledDt * game->GetTimeScale();
		dt = clampf(dt, 0.0001f, 50.0f);


		if (unscaledDt >= tickPerFrame || game->GetTimeScale() == 0)
		{
			game->SetUnscaledDt(now - frameStart);
			//game->SetTimeScale(0.4f);
			DebugOutTitle(L"01 - Skeleton %f", (double)((1.0 / (unscaledDt / 1000.0))));
			frameStart = now;
		
			Update(dt);
			Render();

			game->SwitchScene();
		}
		else
		{
			while (GetTickCount64() - frameStart <= tickPerFrame)
				Sleep((GetTickCount64() - frameStart) / 5);
		}
	}

	UnloadResources();

	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) 
{
	FILE* fp;
	AllocConsole();
	// Redirect STDOUT if the console has an output handle
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);

	srand(time(NULL));
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);


	CGame * game = CGame::GetInstance();
	game->Init(hWnd, hInstance);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 8, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);


	LoadResources();

	Run();

	return 0;
}