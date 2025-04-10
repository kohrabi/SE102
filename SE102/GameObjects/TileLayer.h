#pragma once

#include "Engine/GameObject.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Math/Vector2i.h"
#include "Engine/Graphics/Textures.h"

#include <vector>

using namespace std;

struct Tile
{
	string texturePath;
	Vector2 position;
	RECT textureRegion;

	Tile() {}

	Tile(string texturePath, Vector2 position, RECT textureRegion)
	{
		this->texturePath = texturePath;
		this->position = position;
		this->textureRegion = textureRegion;
	}
};

class CTileLayer : public CGameObject {
private:
	vector<Tile> tiles;
	AABB collision;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CTileLayer() {}
	CTileLayer(AABB collisionRect) : CGameObject(0, 0, 0.0f) {
		collision = collisionRect;
		tiles = vector<Tile>();
	}

	void AddTile(Tile tile) {
		tiles.push_back(tile);
	}

	int IsCollidable() override { return true; };
	int IsDirectionColliable(float nx, float ny) override { return true; }
	
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
		left = (float)collision.left; 
		top = (float)collision.top;
		right = (float)collision.right;
		bottom = (float)collision.bottom;
	}

	void Render() override {
		CGame* const game = CGame::GetInstance();
		CTextures* const textures = CTextures::GetInstance();
		for (auto& tile : tiles)
		{
			game->Draw(tile.position.x, tile.position.y, 0.0f, textures->Get(tile.texturePath), &tile.textureRegion);
		}
		RenderBoundingBox();
	}
};

typedef CTileLayer* LPTILELAYER;