#pragma once

#include "Engine/GameObject.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Math/Vector2.h"

#include <iostream>
using namespace std;

class COneWay : public CGameObject {
private:
    Vector2 size;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	COneWay(float x, float y, float width, float height) 
		: CGameObject(x, y, 0.0f), size(width, height)  {
	}
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}
	void Render() override { RenderBoundingBox(); }

	int IsBlocking() override { return 1; }
	int IsDirectionColliable(float nx, float ny) override { return nx == 0 && ny < 0; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		left = position.x - round(size.x / 2.0f);
		top = position.y - round(size.y / 2.0f);
		right = position.x + round(size.x / 2.0f);
		bottom = position.y + round(size.y / 2.0f);
	}
};