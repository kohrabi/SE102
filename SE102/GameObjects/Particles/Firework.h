#pragma once

#include "Engine/GameObject.h"

class CFirework : public CGameObject {
private:
	static void LoadContent();
	static bool IsContentLoaded;

	float timer = 5000.0f;
	float showDelay = 0.0f;
public:
	CFirework(float x, float y, float showDelay) : CGameObject(x, y) {
		layer = SortingLayer::CORPSE;
		this->showDelay = showDelay;
		LoadContent();
	}

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override { left = 0; top = 0; right = 0; bottom = 0; }
	int IsCollidable() override { return 0; };
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) override { return 0; }
};