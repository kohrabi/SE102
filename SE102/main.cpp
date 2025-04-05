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
#include "GameObjects/CTile.h"


#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <iostream>

#define WINDOW_CLASS_NAME L"Game Window"
#define MAIN_WINDOW_TITLE L"01 - Skeleton"
#define BACKGROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)
#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 246


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


void ClearScene(vector<LPGAMEOBJECT>& objects)
{
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Load all game resources. In this example, create a brick object and mario object
*/
void LoadResources()
{
	CGame* const game = CGame::GetInstance();
	CTextures* const textures = CTextures::GetInstance();
	textures->Add(TEXTURE_PATH_BBOX);
	
	int backBufferWidth = game->GetBackBufferWidth();
	int backBufferHeight = game->GetBackBufferHeight();

	//if (tMap.load(TEST_TILE_TMX)) {
	//	const auto& tilesets = tMap.getTilesets();

	//	const auto& layers = tMap.getLayers();
	//	for (const auto& layer : layers) {
	//		if (layer->getType() == tmx::Layer::Type::Object)
	//		{
	//			const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
	//			const auto& objects = objectLayer.getObjects();
	//			for (const auto& object : objects)
	//			{
	//				const auto& aabb = object.getAABB();
	//				if (object.getClass() == "CPlayer")
	//				{
	//					CPlayer* tank = new CPlayer(aabb.left + aabb.width / 2.0f, aabb.top - aabb.height / 2.0f, 0.f);
	//					game->objects.push_back(tank);
	//				}
	//				else if (object.getClass() == "CTankSpawner") {
	//					float timeOffset = 0.0f;
	//					for (const auto& proper : object.getProperties()) {
	//						if (proper.getName() == "timeOffset")
	//							timeOffset = proper.getFloatValue();
	//					}

	//					CTankSpawner* tank = new CTankSpawner(aabb.left + aabb.width / 2.0f, aabb.top - aabb.height / 2.0f, 0.f, timeOffset);
	//					game->objects.push_back(tank);
	//				}
	//				//do stuff with object properties
	//			}
	//		}
	//		else if (layer->getType() == tmx::Layer::Type::Tile)
	//		{
	//			const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
	//			const auto& layerSize = tileLayer.getSize();
	//			const auto& mapTileSize = tMap.getTileSize();
	//			const auto& mapSize = tMap.getTileCount();
	//			const auto& tiles = tileLayer.getTiles();

	//			for (const auto& tileset : tilesets) {
	//				const auto& tilesetTiles = tileset.getTiles();
	//				for (int i = 0; i < mapSize.x; i++)
	//				{
	//					for (int j = 0; j < mapSize.y; j++) {
	//						int idx = j * mapSize.x + i;
	//						if (idx < tiles.size() && tiles[idx].ID >= tileset.getFirstGID() && tiles[idx].ID <= tileset.getLastGID()) {

	//							auto imagePosition = tilesetTiles[tiles[idx].ID - 1].imagePosition;
	//							auto tileSize = tilesetTiles[tiles[idx].ID - 1].imageSize;
	//							Vector2 offset(tileSize.x / 2.0f, tileSize.y / 2.0f);
	//							game->objects.push_back(new CTile(mapTileSize.x * i + offset.x, mapTileSize.y * j + offset.y, 
	//								textures->Get(STRING_TO_WSTRING(tileset.getImagePath())),
	//								imagePosition.x / mapTileSize.x, imagePosition.y / mapTileSize.y, mapTileSize.x, mapTileSize.y));
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}

	//}

}

void UnloadResources() {
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	CGame* const game = CGame::GetInstance();

	CGame::GetInstance()->GetCurrentScene()->Update(dt);


	DebugOutTitle(L"01 - Skeleton %f", dt); 
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
	pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

	spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	// Use Alpha blending for transparent sprites
	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

	CGame::GetInstance()->GetCurrentScene()->Render();

	// Uncomment this line to see how to draw a porttion of a texture  
	//g->Draw(10, 10, 0, texMisc, 300, 117, 317, 134);
	//g->Draw(10, 10, texMario, 215, 120, 234, 137);

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

	srand(time(nullptr));
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);


	CGame * game = CGame::GetInstance();
	game->Init(hWnd, hInstance);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);


	LoadResources();

	Run();

	return 0;
}