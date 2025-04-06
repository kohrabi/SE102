#pragma once
#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <vector>
#include <unordered_map>

#include "Scene.h"
#include "Graphics/Texture.h"
#include "Engine/GameObject.h"

#define MAX_FRAME_RATE 120
#define KEYBOARD_BUFFER_SIZE 1024
#define KEYBOARD_STATE_SIZE 256

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

/*
	Our simple game framework 
*/
class CGame
{
	static CGame* __instance;
	HWND hWnd = {};									// Window handle

	int backBufferWidth = 0;					// Backbuffer width & height, will be set during Direct3D initialization
	int backBufferHeight = 0;
	float cam_x = 0.0f;
	float cam_y = 0.0f;

	ID3D10Device* pD3DDevice = NULL;
	IDXGISwapChain* pSwapChain = NULL;
	ID3D10RenderTargetView* pRenderTargetView = NULL;
	ID3D10BlendState* pBlendStateAlpha = NULL;			// To store alpha blending state
	ID3D10SamplerState* pPointSamplerState;

	HINSTANCE hInstance;

	unordered_map<int, LPSCENE> scenes = {};
	int current_scene = {};
	int next_scene = -1;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

	ID3DX10Sprite* spriteObject = NULL;				// Sprite handling object 
	bool keyState[256] = {};
	bool prevKeyState[256] = {};

public:
	bool leftMouseDown;

	CGame() {}
	CGame(const CGame&) = delete;
	void operator=(const CGame&) = delete;

	// Init DirectX, Sprite Handler
	void Init(HWND hWnd, HINSTANCE hInstance);

	void SetPointSamplerState();

	//
	// Draw a portion or ALL the texture at position (x,y) on the screen
	// rect : if NULL, the whole texture will be drawn
	//        if NOT NULL, only draw that portion of the texture 
	void Draw(float x, float y, float rotation, LPTEXTURE tex, RECT *rect = NULL, bool flipX = false, bool flipY = false, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0);

	void Draw(float x, float y, float rotation, LPTEXTURE tex, int l, int t, int r, int b, bool flipX = false, bool flipY = false, float alpha = 1.0f, int sprite_width = 0, int sprite_height = 0)
	{
		RECT rect; 
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		this->Draw(x, y, rotation, tex, &rect, flipX, flipY, alpha, sprite_width, sprite_height);
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

	LPSCENE GetCurrentScene() { return scenes[current_scene]; }

	void Load(LPCWSTR gameFile);
	void SwitchScene();
	void InitiateSwitchScene(int scene_id);

	void _ParseSection_TEXTURES(string line);

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }

	int GetBackBufferWidth() { return backBufferWidth; }
	int GetBackBufferHeight() { return backBufferHeight; }


	static CGame* const GetInstance();

	~CGame();
};
typedef CGame* LPGAME;
