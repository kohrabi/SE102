#include "debug.h"
#include "Engine/Game.h"
#include "Utils.h"
#include "PlayScene.h"
#include "Graphics/Sprites.h"
#include "Graphics/Animations.h"
#include "Graphics/Textures.h"

#include "Math/AABB.h"

#include <fstream>

CGame* CGame::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for 
	rendering 2D images
	- hWnd: Application window handle
*/
void CGame::Init(HWND hWnd, HINSTANCE hInstance)
{

	this->hWnd = hWnd;
	this->hInstance = hInstance;

	// retrieve client area width & height so that we can create backbuffer height & width accordingly 
	RECT r;
	GetClientRect(hWnd, &r);

	backBufferWidth = r.right + 1;
	backBufferHeight = r.bottom + 1;

	// Create & clear the DXGI_SWAP_CHAIN_DESC structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill in the needed values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = backBufferWidth;
	swapChainDesc.BufferDesc.Height = backBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// Create the D3D device and the swap chain
	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pD3DDevice);

	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3D10CreateDeviceAndSwapChain has failed %s %d", _W(__FILE__), __LINE__);
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

	pBackBuffer->Release();
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] CreateRenderTargetView has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// set the render target
	pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// create and set the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = backBufferWidth;
	viewPort.Height = backBufferHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pD3DDevice->RSSetViewports(1, &viewPort);

	D3D10_SAMPLER_DESC desc;
	desc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT; 
	desc.AddressU = D3D10_TEXTURE_ADDRESS_MIRROR;
	desc.AddressV = D3D10_TEXTURE_ADDRESS_MIRROR;
	desc.AddressW = D3D10_TEXTURE_ADDRESS_MIRROR;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D10_COMPARISON_NEVER;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;

	pD3DDevice->CreateSamplerState(&desc, &this->pPointSamplerState);


	// create the sprite object to handle sprite drawing 
	hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteObject);

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


	// Initialize the blend state for alpha drawing
	D3D10_BLEND_DESC StateDesc;
	ZeroMemory(&StateDesc, sizeof(D3D10_BLEND_DESC));
	StateDesc.AlphaToCoverageEnable = FALSE;
	StateDesc.BlendEnable[0] = TRUE;
	StateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	StateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	StateDesc.BlendOp = D3D10_BLEND_OP_ADD;
	StateDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	StateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	pD3DDevice->CreateBlendState(&StateDesc, &this->pBlendStateAlpha);



	DebugOut((wchar_t*)L"[INFO] InitDirectX has been successful\n");

	return;
}

void CGame::SetPointSamplerState()
{
	// pD3DDevice->VSSetSamplers(0, 1, &pPointSamplerState);
	// pD3DDevice->GSSetSamplers(0, 1, &pPointSamplerState);
	pD3DDevice->PSSetSamplers(0, 1, &pPointSamplerState);
}

/*
	Draw the whole texture or part of texture onto screen
	NOTE: This function is OBSOLTED in this example. Use Sprite::Render instead 
*/
void CGame::Draw(float x, float y, float rotation, float z, LPTEXTURE tex, RECT* rect, bool flipX, bool flipY, float alpha, int sprite_width, int sprite_height)
{
	if (tex == NULL) return; 

	int spriteWidth = sprite_width;
	int spriteHeight = sprite_height;

	float cx, cy;
	GetCamPos(cx, cy);
	cx = roundf(cx);
	cy = roundf(cy);
	
	x = roundf(x);
	y = roundf(y);

	D3DX10_SPRITE sprite;

	// Set the sprite�s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	if (rect==NULL) 
	{

		if (spriteWidth==0) spriteWidth = tex->getWidth();
		if (spriteHeight==0) spriteHeight = tex->getHeight();

		// top-left location in U,V coords
		if (flipX) {
			sprite.TexCoord.x = 1.0f;
			sprite.TexSize.x = -1.0f;
		}
		else {
			sprite.TexCoord.x = 0;
			sprite.TexSize.x = 1.0f;
		}

		if (flipY) {
			sprite.TexCoord.y = 1.0f;
			sprite.TexSize.y = -1.0f;
		}
		else {
			sprite.TexCoord.y = 0;
			sprite.TexSize.y = 1.0f;
		}

		// Determine the texture size in U,V coords
	}
	else
	{
		if (spriteWidth == 0) spriteWidth = (rect->right - rect->left + 1);
		if (spriteHeight == 0) spriteHeight = (rect->bottom - rect->top + 1);
		
		if (flipX) {
			// Flip the UV from 0-1 to -1-0
			sprite.TexCoord.x = (rect->right + 1) / (float)tex->getWidth();
			sprite.TexSize.x = -spriteWidth / (float)tex->getWidth();
		}
		else {
			sprite.TexCoord.x = rect->left / (float)tex->getWidth();
			sprite.TexSize.x = spriteWidth / (float)tex->getWidth();
		}

		if (flipY) {
			sprite.TexCoord.y = (rect->bottom + 1) / (float)tex->getHeight();
			sprite.TexSize.y = -spriteHeight / (float)tex->getHeight();
		}
		else {
			sprite.TexCoord.y = rect->top / (float)tex->getHeight();
			sprite.TexSize.y = spriteHeight / (float)tex->getHeight();
		}
	}

	// Skip if not in view
	AABB cam, spriteRect;
	cam.left = cx;
	cam.right = (cx + backBufferWidth);
	cam.top = cy;
	cam.bottom = (cy + backBufferHeight);

	spriteRect.left = (x - spriteWidth / 2.0f);
	spriteRect.right = (x + spriteWidth / 2.0f);
	spriteRect.top = (y - spriteHeight / 2.0f);
	spriteRect.bottom = (y + spriteHeight / 2.0f);

	if (!CCollision::CheckAABBOverlaps(cam, spriteRect))
		return;

	// Set the texture index. Single textures will use 0
	sprite.TextureIndex = 0;


	// The color to apply to this sprite, full color applies white.
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

	//
	// Build the rendering matrix based on sprite location 
	//

	// The translation matrix to be created
	D3DXMATRIX matTranslation;

	// Create the translation matrix
	// ------------------------------------------ CURRENT ROUND THE POSITION FOR PIXEL ART
	D3DXMatrixTranslation(&matTranslation, roundf(x - cx), roundf(backBufferHeight - y + cy), 0.1f);

	D3DXMATRIX matRotation;
	//D3DXMatrixIdentity(&matRotation);
	D3DXMatrixRotationZ(&matRotation, rotation);

	// Scale the sprite to its correct width and height because by default, DirectX draws it with width = height = 1.0f 
	D3DXMATRIX matScaling;
	D3DXMatrixScaling(&matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);

	// Setting the sprite�s position and size
	sprite.matWorld = (matScaling * matRotation * matTranslation);

	// SetPointSamplerState();

	spriteObject->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

void CGame::DrawScreen(float x, float y, float rotation, float z, LPTEXTURE tex, RECT* rect, bool flipX, bool flipY, float alpha, int sprite_width, int sprite_height)
{
	if (tex == NULL) return;

	int spriteWidth = sprite_width;
	int spriteHeight = sprite_height;

	x = roundf(x);
	y = roundf(y);

	D3DX10_SPRITE sprite;

	// Set the sprite�s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	if (rect == NULL)
	{

		if (spriteWidth == 0) spriteWidth = tex->getWidth();
		if (spriteHeight == 0) spriteHeight = tex->getHeight();

		// top-left location in U,V coords
		if (flipX) {
			sprite.TexCoord.x = 1.0f;
			sprite.TexSize.x = -1.0f;
		}
		else {
			sprite.TexCoord.x = 0;
			sprite.TexSize.x = 1.0f;
		}

		if (flipY) {
			sprite.TexCoord.y = 1.0f;
			sprite.TexSize.y = -1.0f;
		}
		else {
			sprite.TexCoord.y = 0;
			sprite.TexSize.y = 1.0f;
		}

		// Determine the texture size in U,V coords
	}
	else
	{
		if (spriteWidth == 0) spriteWidth = (rect->right - rect->left + 1);
		if (spriteHeight == 0) spriteHeight = (rect->bottom - rect->top + 1);

		if (flipX) {
			// Flip the UV from 0-1 to -1-0
			sprite.TexCoord.x = (rect->right + 1) / (float)tex->getWidth();
			sprite.TexSize.x = -spriteWidth / (float)tex->getWidth();
		}
		else {
			sprite.TexCoord.x = rect->left / (float)tex->getWidth();
			sprite.TexSize.x = spriteWidth / (float)tex->getWidth();
		}

		if (flipY) {
			sprite.TexCoord.y = (rect->bottom + 1) / (float)tex->getHeight();
			sprite.TexSize.y = -spriteHeight / (float)tex->getHeight();
		}
		else {
			sprite.TexCoord.y = rect->top / (float)tex->getHeight();
			sprite.TexSize.y = spriteHeight / (float)tex->getHeight();
		}
	}

	// Set the texture index. Single textures will use 0
	sprite.TextureIndex = 0;


	// The color to apply to this sprite, full color applies white.
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

	//
	// Build the rendering matrix based on sprite location 
	//

	// The translation matrix to be created
	D3DXMATRIX matTranslation;

	// Create the translation matrix
	// ------------------------------------------ CURRENT ROUND THE POSITION FOR PIXEL ART
	D3DXMatrixTranslation(&matTranslation, roundf(x), roundf(backBufferHeight - y), 0.1f);

	D3DXMATRIX matRotation;
	//D3DXMatrixIdentity(&matRotation);
	D3DXMatrixRotationZ(&matRotation, rotation);

	// Scale the sprite to its correct width and height because by default, DirectX draws it with width = height = 1.0f 
	D3DXMATRIX matScaling;
	D3DXMatrixScaling(&matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);

	// Setting the sprite�s position and size
	sprite.matWorld = (matScaling * matRotation * matTranslation);

	// SetPointSamplerState();

	spriteObject->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

/*
	Utility function to wrap D3DX10CreateTextureFromFileEx
*/
LPTEXTURE CGame::LoadTexture(LPCWSTR texturePath)
{
	ID3D10Resource* pD3D10Resource = NULL;
	ID3D10Texture2D* tex = NULL;

	// Retrieve image information first 
	D3DX10_IMAGE_INFO imageInfo;
	HRESULT hr = D3DX10GetImageInfoFromFile(texturePath, NULL, &imageInfo, NULL);
	if (FAILED(hr))
	{
		DebugOut((wchar_t*)L"[ERROR] D3DX10GetImageInfoFromFile failed for  file: %s with error: %d\n", texturePath, hr);
		return NULL;
	}

	D3DX10_IMAGE_LOAD_INFO info;
	ZeroMemory(&info, sizeof(D3DX10_IMAGE_LOAD_INFO));
	info.Width = imageInfo.Width;
	info.Height = imageInfo.Height;
	info.Depth = imageInfo.Depth;
	info.FirstMipLevel = 0;
	info.MipLevels = 1;
	info.Usage = D3D10_USAGE_DEFAULT;
	info.BindFlags = D3DX10_DEFAULT;
	info.CpuAccessFlags = D3DX10_DEFAULT;
	info.MiscFlags = D3DX10_DEFAULT;
	info.Format = imageInfo.Format;
	info.Filter = D3DX10_FILTER_NONE;
	info.MipFilter = D3DX10_DEFAULT;
	info.pSrcInfo = &imageInfo;

	// Loads the texture into a temporary ID3D10Resource object
	hr = D3DX10CreateTextureFromFile(pD3DDevice,
		texturePath,
		&info,
		NULL,
		&pD3D10Resource,
		NULL);

	// Make sure the texture was loaded successfully
	if (FAILED(hr))
	{
		DebugOut((wchar_t*)L"[ERROR] Failed to load texture file: %s with error: %d\n", texturePath, hr);
		return NULL;
	}

	// Translates the ID3D10Resource object into a ID3D10Texture2D object
	pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&tex);
	pD3D10Resource->Release();

	if (!tex) {
		DebugOut((wchar_t*)L"[ERROR] Failed to convert from ID3D10Resource to ID3D10Texture2D \n");
		return NULL;
	}

	//
	// Create the Share Resource View for this texture 
	// 	   
	// Get the texture details
	D3D10_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

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

	pD3DDevice->CreateShaderResourceView(tex, &SRVDesc, &gSpriteTextureRV);

	DebugOut(L"[INFO] Texture loaded Ok from file: %s \n", texturePath);

	return new CTexture(tex, gSpriteTextureRV);
}

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2
#define GAME_FILE_SECTION_TEXTURES 3


void CGame::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		next_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting: %s\n", ToWSTR(tokens[0]).c_str());
}

void CGame::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);   // file: ASCII format (single-byte char) => Wide Char

	LPSCENE scene = new CPlayScene(id, path);
	scenes[id] = scene;
}

/*
	Load game campaign file and load/initiate first scene
*/
void CGame::Load(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f{};
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[TEXTURES]") { section = GAME_FILE_SECTION_TEXTURES; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }
		if (line[0] == '[') 
		{ 
			section = GAME_FILE_SECTION_UNKNOWN; 
			DebugOut(L"[ERROR] Unknown section: %s\n", ToWSTR(line).c_str());
			continue; 
		}

		//
		// data section
		//
		switch (section)
		{
		case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		case GAME_FILE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n", gameFile);

	SwitchScene();
}

void CGame::SwitchScene()
{
	if (next_scene < 0 || next_scene == current_scene) return; 

	DebugOut(L"[INFO] Switching to scene %d\n", next_scene);

	scenes[current_scene]->Unload();

	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();

	current_scene = next_scene;
	LPSCENE s = scenes[next_scene];
	s->Load();
}


void CGame::ResetScene()
{
	if (!resetScene)
	{
		DebugOut(L"[ERROR] Cannot reset scene this->resetScene is false\n");
		return;
	}
	DebugOut(L"[INFO] Reset scene %d\n", current_scene);

	scenes[current_scene]->Unload();

	resetScene = false;
	LPSCENE s = scenes[current_scene];
	s->Load();

	timeScale = 1.0f;

}

void CGame::InitiateSwitchScene(int scene_id)
{
	next_scene = scene_id;
}


void CGame::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;

	wstring path = ToWSTR(tokens[1]);

	CTextures::GetInstance()->Add(path.c_str());
}


CGame::~CGame()
{
	pBlendStateAlpha->Release();
	spriteObject->Release();
	pRenderTargetView->Release();
	pSwapChain->Release();
	pD3DDevice->Release();
}

CGame* const CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}
