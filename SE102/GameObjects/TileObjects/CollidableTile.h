#pragma once

#include "Engine/GameObject.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Math/Vector2i.h"

class CCollidableTile : public CTile {
private:
	RECT textureRegion;
	LPTEXTURE texture;
	Vector2i tileSize;
public:
	CCollidableTile(float x, float y, LPTEXTURE texture, int xTile, int yTile, int tileSizeX, int tileSizeY) 
		: CTile(x, y, texture, xTile, yTile, tileSizeX, tileSizeY) {
		layer = SortingLayer::WALL;
	}

	void Render() override {
		CTile::Render();
		RenderBoundingBox();
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		left = position.x - round(tileSize.x / 2.0f);
		top = position.y - round(tileSize.y / 2.0f);
		right = position.x + round(tileSize.x / 2.0f);
		bottom = position.y + round(tileSize.y / 2.0f);
	}
};