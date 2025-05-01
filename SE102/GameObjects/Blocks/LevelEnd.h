#pragma once

#include "Engine/GameObject.h"
#include "Engine/debug.h"

#include "Engine/CollisionCast.h"

#define LEVEL_END_STATE_NORMAL 1
#define LEVEL_END_STATE_EMPTY 2

class CLevelEnd : public CGameObject {
private:

	static bool IsContentLoaded;
	static void LoadContent();

	float yOffset = 0.0f;
	int currentRewardSlot = 0;
public:

	CLevelEnd(float x, float y);
	void Render() override;
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	
	int IsCollidable() override { return !isDeleted; }
	int IsBlocking() override { return false; }
	void SetState(int state) override;
	void Hit();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		const float size = 16.0f / 2.0f;
		left = position.x - size;
		top = position.y - size;
		right = position.x + size;
		bottom = position.y + size;
	}
};