#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <vector>

#include "Graphics/Texture.h"
#include "Engine/GameObject.h"

#define MAX_FRAME_RATE 60
#define KEYBOARD_BUFFER_SIZE 1024
#define KEYBOARD_STATE_SIZE 256

/*
	Our simple game framework 
*/
class CGame
{
	static CGame* __instance;
	HWND hWnd;									// Window handle

	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;
	int viewportWidth = 0;
	int viewportHeight = 0;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state
	ID3D10SamplerState* pPointSamplerState;

	HINSTANCE hInstance;

	ID3DX10Sprite* spriteObject = NULL;				// Sprite handling object 
	bool keyState[256];
	bool prevKeyState[256];

public:
	// Init DirectX, Sprite Handler
	void Init(HWND hWnd, HINSTANCE hInstance);

	void SetPointSamplerState();

	std::vector<LPGAMEOBJECT> objects;
	LPTEXTURE texBullet = NULL;
	LPTEXTURE texBTSprites = NULL;

	void WindowResized(UINT width, UINT height);
	//
	// Draw a portion or ALL the texture at position (x,y) on the screen
	// rect : if NULL, the whole texture will be drawn
	//        if NOT NULL, only draw that portion of the texture 
	void Draw(float x, float y, float rotation, LPTEXTURE tex, RECT *rect = NULL);

	void Draw(float x, float y, float rotation, LPTEXTURE tex, int l, int t, int r, int b)
	{
		RECT rect; 
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		this->Draw(x, y, rotation, tex, &rect);
	}

	LPTEXTURE LoadTexture(LPCWSTR texturePath);

	ID3D10Device * GetDirect3DDevice() { return this->pD3DDevice; }
	IDXGISwapChain* GetSwapChain() { return this->pSwapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return this->pRenderTargetView;  }
	ID3DX10Sprite* GetSpriteHandler() { return this->spriteObject; }
	ID3D10BlendState* GetAlphaBlending() { return pBlendStateAlpha; };

	POINT GetMousePosition(HWND hwnd)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);          // Get mouse position in screen coordinates
		ScreenToClient(hwnd, &mousePos);  // Convert to client area coordinates
		return mousePos;
	}

	bool IsKeyDown(int keyCode) { return keyState[keyCode]; }
	bool IsKeyJustPressed(int keyCode) { return keyState[keyCode] && !prevKeyState[keyCode]; }
	bool IsKeyJustReleased(int keyCode) { return !keyState[keyCode] && prevKeyState[keyCode]; }

	void SetKeyState(int keyCode, bool state) { keyState[keyCode] = state; }
	void SetPrevKeyState() {
		for (int i = 0; i < 256; i++)
			prevKeyState[i] = keyState[i]; 
	}

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }

	int GetViewportWidth() { return viewportWidth; }
	int GetViewportHeight() { return viewportHeight; }
	bool leftMouseDown;

	static CGame* const GetInstance();

	~CGame();
};