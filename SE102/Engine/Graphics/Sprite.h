#pragma once

#include "Texture.h"
#include "Engine/Game.h"

class CSprite
{
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;
	

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex);
	CSprite(int id, RECT r, LPTEXTURE tex) : CSprite(id, r.left, r.top, r.right, r.bottom, tex) {}

	int getWidth() { 
		if (right - left != 0)
			return right - left; 
		return texture->getWidth();
	}
	int getHeight() { 
		if (bottom - top != 0)	
			return bottom - top; 
		return texture->getHeight();
	}

	void Draw(float x, float y, float z, bool flipX = false, bool flipY = false);
	void DrawScreen(float x, float y, float z, bool flipX = false, bool flipY = false);
};

typedef CSprite* LPSPRITE;