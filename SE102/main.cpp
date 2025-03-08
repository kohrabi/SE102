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

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "CTank.h"
#include "CPlayer.h"
#include "CTankEnemy.h"
#include "CTankEnemyRed.h"


#define WINDOW_CLASS_NAME L"Game Window"
#define MAIN_WINDOW_TITLE L"01 - Skeleton"
#define TEXTURE_PATH_BTSPRITES L"btSprites.png"

#define BACKGROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240


using namespace std;

CMario *mario;
#define MARIO_START_X 10.0f
#define MARIO_START_Y 100.0f
#define MARIO_START_VX 0.1f
#define MARIO_START_VY 0.1f


CBrick *brick;
#define BRICK_X 10.0f
#define BRICK_Y 120.0f

LPTEXTURE texMario = NULL;
LPTEXTURE texBrick = NULL;
LPTEXTURE texMisc = NULL;
LPTEXTURE texShip = NULL;
LPTEXTURE texEnemy = NULL;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		CGame::GetInstance()->keyState[wParam] = true;
		CGame::GetInstance()->keyJustPressed[wParam] = true;
		break;
	case WM_KEYUP:
		CGame::GetInstance()->keyState[wParam] = false;
		CGame::GetInstance()->keyJustPressed[wParam] = false;
		break;
	//case WM_SIZE:
	//	{
	//		RECT r;
	//		GetClientRect(hWnd, &r);
	//		UINT wR = r.right + 1;
	//		UINT hR = r.bottom + 1;
	//		UINT width = LOWORD(lParam);
	//		UINT height = HIWORD(lParam);
	//		CGame::GetInstance()->WindowResized(width, height);
	//	}
	//	return 0;
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
	CGame * game = CGame::GetInstance();
	game->texBTSprites = game->LoadTexture(TEXTURE_PATH_BTSPRITES);
	//game->objects = std::vector(0);

	// Load a sprite sheet as a texture to try drawing a portion of a texture. See function Render 
	//texMisc = game->LoadTexture(MISC_TEXTURE_PATH);

	//mario = new CMario(MARIO_START_X, MARIO_START_Y, MARIO_START_VX, MARIO_START_VY, texMario);
	//brick = new CBrick(0, 0, texBrick);

	CPlayer* tank = new CPlayer(50.f, 50.f, 0.f, 0.f, 0.f, game->texBTSprites);
	game->objects.push_back(tank);
	for (int i = 0; i < 1; i++) {
		game->objects.push_back(new CTankEnemyRed(35.f * i, 32.f * i, 0.f, 0.f, 0.f, game->texBTSprites, 0.5f * i));
	}
	for (int i = 0; i < 7; i++) {
		game->objects.push_back(new CTankEnemy(20.f * i, 30.f * i, 0.f, 0.f, 0.f, game->texBTSprites, 0.5f * i));
	}
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
float PointDistance(float x, float y) {
	return sqrt(x * x + y * y);
}

bool checkCircle(float ax, float ay, float radiusA, float cx, float cy, float radiusC) {
	return PointDistance(cx - ax, cy - ay) <= radiusA + radiusC;
}
void Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();

	for (int i = 0; i < game->objects.size(); i++)
		//if (!game->objects[i]->ShouldDestroy())
		game->objects[i]->Update(dt);	

	/*for (int i = 0; i < game->objects.size(); i++) {
		LPGAMEOBJECT objectA = game->objects[i];
		if (objectA->ShouldDestroy())
			continue;
		for (int j = 0; j < game->objects.size(); j++) {
			if (i == j)
				continue;
			LPGAMEOBJECT objectB = game->objects[j];
			if (objectB->ShouldDestroy())
				continue;
			auto& test = typeid(*objectA);
			if (std::is_same_v<decltype(objectA), CTank*> && std::is_same_v<decltype(*objectB), CTankBullet*>) {
				DebugOut(L"Hello\n");
			}
		}

	}*/

	for (int i = 0; i < 256; i++)
		game->keyJustPressed[i] = false;
	//for (int i = 0; i < game->objects.size(); i++) {
	//	LPGAMEOBJECT objectA = game->objects[i];
	//	for (int j = 0; j < game->objects.size(); j++) {
	//		if (i == j) continue;
	//		LPGAMEOBJECT objectB = game->objects[j];
	//		if (typeid(objectA) == typeid(CBullet) && typeid(objectB) == typeid(CEnemy))
	//		{

	//			if (checkCircle(objectA->GetX(), objectB->GetY(), 30, objectB->GetX(), objectB->GetY(), 30)) {

	//			}
	//		}
	//	}
	//}

	/*mario->Update(dt);
	brick->Update(dt);*/

	DebugOutTitle(L"01 - Skeleton %f", dt); 
}

/*
	Render a frame 
*/
void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		// clear the background 
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		// Use Alpha blending for transparent sprites
		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		for (int i = 0; i < g->objects.size(); i++) {
			if (!g->objects[i]->ShouldDestroy())
				g->objects[i]->Render();
		}

		//brick->Render();
		//mario->Render();

		// Uncomment this line to see how to draw a porttion of a texture  
		//g->Draw(10, 10, 0, texMisc, 300, 117, 317, 134);
		//g->Draw(10, 10, texMario, 215, 120, 234, 137);

		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
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
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

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

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) 
{
	srand(time(NULL));
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame * game = CGame::GetInstance();
	game->Init(hWnd);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);


	LoadResources();

	Run();

	return 0;
}