#pragma once

#include "Engine/GameObject.h"
#include "Engine/Graphics/Sprites.h"

class CTile : public CGameObject {
private:
	RECT textureRegion;
	LPTEXTURE texture;
	Vector2 tileSize;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CTile(float x, float y, LPTEXTURE texture, int xTile, int yTile, int tileSizeX, int tileSizeY) 
		: CGameObject(x, y, 0.0f), texture(texture), tileSize(tileSizeX, tileSizeY)  {
		textureRegion = GetTextureRegion(xTile, yTile, tileSizeX, tileSizeY);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}
	void Render() override {
		CGame* const game = CGame::GetInstance();
		//game->Draw(position.x, position.y, 0.0f, texture, &textureRegion);
		RenderBoundingBox();
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		left = position.x - round(tileSize.x / 2);
		top = position.y - round(tileSize.y / 2);
		right = position. x + round(tileSize.x / 2);
		bottom = position.y + round(tileSize.y / 2);
	}
};