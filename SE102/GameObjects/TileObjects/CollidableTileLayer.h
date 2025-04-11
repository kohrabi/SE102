#pragma once

#include "TileLayer.h"

#include <vector>

using namespace std;

class CCollidableTileLayer : public CTileLayer {
private:
	AABB collision{ 0 };
public:
	CCollidableTileLayer() : CTileLayer() {}
	CCollidableTileLayer(AABB collisionRect) : CTileLayer() { collision = collisionRect; }

	int IsCollidable() override { return true; };
	int IsDirectionColliable(float nx, float ny) override { return true; }
	
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
		left = (float)collision.left; 
		top = (float)collision.top;
		right = (float)collision.right;
		bottom = (float)collision.bottom;
	}

	void Render() override {
		CTileLayer::Render();
		RenderBoundingBox();
	}
};

typedef CCollidableTileLayer* LPCOLLIDABLETILELAYER;