#include "debug.h"
#include "Engine/Game.h"

CGame* CGame::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for 
	rendering 2D images
	- hWnd: Application window handle
*/
void CGame::Init(HWND hWnd)
{
	// retrieve client area width & height so that we can create backbuffer height & width accordingly 
	RECT r;
	GetClientRect(hWnd, &r);

	backBufferWidth = r.right + 1;
	backBufferHeight = r.bottom + 1;
	viewportWidth = backBufferWidth;
	viewportHeight = backBufferHeight;

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

	//D3D10_SAMPLER_DESC desc;
	//desc.Filter = D3D10_FILTER_MIN_MAG_MIP_POINT; 
	//desc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
	//desc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
	//desc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
	//desc.MipLODBias = 0;
	//desc.MaxAnisotropy = 1;
	//desc.ComparisonFunc = D3D10_COMPARISON_NEVER;
	//desc.BorderColor[0] = 1.0f;
	//desc.BorderColor[1] = 1.0f;
	//desc.BorderColor[2] = 1.0f;
	//desc.BorderColor[3] = 1.0f;
	//desc.MinLOD = -FLT_MAX;
	//desc.MaxLOD = FLT_MAX;

	//pD3DDevice->CreateSamplerState(&desc, &this->pPointSamplerState);


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
	pD3DDevice->VSSetSamplers(0, 1, &pPointSamplerState);
	pD3DDevice->GSSetSamplers(0, 1, &pPointSamplerState);
	pD3DDevice->PSSetSamplers(0, 1, &pPointSamplerState);
}

void CGame::WindowResized(UINT width, UINT height)
{
	if (pSwapChain)
	{
		pD3DDevice->OMSetRenderTargets(0, NULL, NULL);

		// Release all outstanding references to the swap chain's buffers.
		pRenderTargetView->Release();
		backBufferWidth = width;
		backBufferHeight = height;

		HRESULT hr;
		// Preserve the existing buffer count and format.
		// Automatically choose the width and height to match the client rect for HWNDs.
		hr = pSwapChain->ResizeBuffers(1, backBufferWidth, backBufferHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		if (hr != S_OK) {
			DebugOut((wchar_t*)L"[ERROR] pSwapChain->ResizeBuffers has failed %s %d", _W(__FILE__), __LINE__);
			DebugBreak();
		}
		// Perform error handling here!

		// Get the back buffer from the swapchain
		ID3D10Texture2D* pBackBuffer = nullptr;
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
		if (hr != S_OK)
		{
			DebugOut((wchar_t*)L"[ERROR] pSwapChain->GetBuffer has failed %s %d", _W(__FILE__), __LINE__);
			DebugBreak();
		}

		// create the render target view
		hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

		pBackBuffer->Release();
		if (hr != S_OK)
		{
			DebugOut((wchar_t*)L"[ERROR] CreateRenderTargetView has failed %s %d", _W(__FILE__), __LINE__);
			DebugBreak();
		}

		pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);


		// 4:3 scale
		const float targetWidth = 320.f;
		const float targetHeight = 240.f;
		float targetAspectRatio = targetWidth / targetHeight;
		float windowAspectRatio = (float)width / (float)height;

		viewportWidth = width;
		viewportHeight = height;
		UINT viewportX = 0, viewportY = 0;

		// ChatGPT'ed
		if (windowAspectRatio > targetAspectRatio) {
			// Window is wider than target - letterbox left and right
			viewportHeight = height;
			viewportWidth = (UINT)(targetAspectRatio * viewportHeight);
			viewportX = (width - viewportWidth) / 2;
			viewportY = 0;
		}
		else {
			// Window is taller than target - letterbox top and bottom
			viewportWidth = width;
			viewportHeight = (UINT)(viewportWidth / targetAspectRatio);
			viewportX = 0;
			viewportY = (height - viewportHeight) / 2;
		}

		// create and set the viewport
		D3D10_VIEWPORT viewport;
		viewport.Width = (UINT)viewportWidth;
		viewport.Height = (UINT)viewportHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = viewportX;
		viewport.TopLeftY = viewportY;
		pD3DDevice->RSSetViewports(1, &viewport);

		D3DXMATRIX matProjection;

		// Create the projection matrix using the values in the viewport
		/*D3DXMatrixOrthoOffCenterLH(&matProjection,
			(float)0,
			(float)viewport.Width,
			(float)0,
			(float)viewport.Height,
			0.1f,
			10);*/
		D3DXMatrixOrthoOffCenterLH(&matProjection,
			(float)0,
			(float)targetWidth,
			(float)0,
			(float)targetHeight,
			0.1f,
			10);
		//D3DXMatrixOrthoOffCenterLH(&matProjection,
		//	(float)viewport.TopLeftX,
		//	(float)viewport.Width,
		//	(float)viewport.Height,
		//	(float)viewport.Height,
		//	0.1f,
		//	10);
		hr = spriteObject->SetProjectionTransform(&matProjection);

		DebugOut(L"Viewport set to width=%d, height=%d, TopLeftX=%f, TopLeftY=%f\n",
			viewport.Width, viewport.Height, viewport.TopLeftX, viewport.TopLeftY);
	}
}

/*
	Draw the whole texture or part of texture onto screen
	NOTE: This function is OBSOLTED in this example. Use Sprite::Render instead 
*/
void CGame::Draw(float x, float y, float rotation, LPTEXTURE tex, RECT* rect)
{
	if (tex == NULL) return; 

	int spriteWidth = 0;
	int spriteHeight = 0;

	D3DX10_SPRITE sprite;

	// Set the sprite�s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	if (rect==NULL) 
	{
		// top-left location in U,V coords
		sprite.TexCoord.x = 0;
		sprite.TexCoord.y = 0;

		// Determine the texture size in U,V coords
		sprite.TexSize.x = 1.0f;
		sprite.TexSize.y = 1.0f;

		spriteWidth = tex->getWidth();
		spriteHeight = tex->getHeight();
	}
	else
	{
		sprite.TexCoord.x = rect->left / (float)tex->getWidth();
		sprite.TexCoord.y = rect->top / (float)tex->getHeight();

		spriteWidth = (rect->right - rect->left + 1);
		spriteHeight = (rect->bottom - rect->top + 1);

		sprite.TexSize.x = spriteWidth / (float)tex->getWidth();
		sprite.TexSize.y = spriteHeight / (float)tex->getHeight();
	}

	// Set the texture index. Single textures will use 0
	sprite.TextureIndex = 0;

	// The color to apply to this sprite, full color applies white.
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//
	// Build the rendering matrix based on sprite location 
	//

	// The translation matrix to be created
	D3DXMATRIX matTranslation;

	// Create the translation matrix
	// ------------------------------------------ CURRENT FLOORING THE POSITION FOR PIXEL ART
	D3DXMatrixTranslation(&matTranslation, roundf(x), roundf(backBufferHeight - y), 0.1f);

	D3DXMATRIX matRotation;
	//D3DXMatrixIdentity(&matRotation);
	D3DXMatrixRotationZ(&matRotation, rotation);

	// Scale the sprite to its correct width and height because by default, DirectX draws it with width = height = 1.0f 
	D3DXMATRIX matScaling;
	D3DXMatrixScaling(&matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);

	// Setting the sprite�s position and size
	sprite.matWorld = (matScaling * matRotation * matTranslation);

	spriteObject->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

/*
	Utility function to wrap D3DX10CreateTextureFromFileEx
*/
LPTEXTURE CGame::LoadTexture(LPCWSTR texturePath)
{
	ID3D10Resource* pD3D10Resource = NULL;
	ID3D10Texture2D* tex = NULL;

	// Loads the texture into a temporary ID3D10Resource object
	HRESULT hr = D3DX10CreateTextureFromFile(pD3DDevice,
		texturePath,
		NULL, //&info,
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

CGame::~CGame()
{

}

CGame* const CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}
