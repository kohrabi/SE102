#pragma once

#include "Engine/GameObject.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Math/Vector2i.h"
#include "Engine/Graphics/Textures.h"

#include <vector>

#include "TileStruct.h"

using namespace std;

class CTileLayer : public CGameObject {
protected:
	vector<Tile> tiles;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CTileLayer() : CGameObject(0, 0, 0.0f) {
		tiles = vector<Tile>();
	}

	void AddTile(Tile tile) { tiles.push_back(tile); }

	int IsCollidable() override { return false; };
	int IsDirectionColliable(float nx, float ny) override { return false; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { left = 0; top = 0; right = 0; bottom = 0; }

	void Render() override {
		CGame* const game = CGame::GetInstance();
		CTextures* const textures = CTextures::GetInstance();
		for (auto& tile : tiles)
		{
			game->Draw(tile.position.x, tile.position.y, 0.0f, textures->Get(tile.texturePath), &tile.textureRegion);
		}
	}
};