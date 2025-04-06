#pragma once

#include "Engine/GameObject.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Math/Vector2i.h"

class CTile : public CGameObject {
private:
	RECT textureRegion;
	LPTEXTURE texture;
	Vector2i tileSize;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CTile(float x, float y, LPTEXTURE texture, int xTile, int yTile, int tileSizeX, int tileSizeY) 
		: CGameObject(x, y, 0.0f), texture(texture), tileSize(tileSizeX, tileSizeY)  {
		textureRegion = GetTextureRegion(xTile, yTile, tileSizeX, tileSizeY);
	}
	int IsCollidable() override { return false; };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { left = 0; top = 0; right = 0; bottom = 0; }
	void Render() override {
		CGame* const game = CGame::GetInstance();
		game->Draw(position.x, position.y, 0.0f, texture, &textureRegion);
	}
};