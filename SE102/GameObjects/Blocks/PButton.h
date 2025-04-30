#pragma once

#include "Engine/GameObject.h"
#include "Engine/debug.h"

#include "Engine/CollisionCast.h"

constexpr float PUFF_TIME = 400.0f;

#define P_BUTTON_STATE_ACTIVE 1
#define P_BUTTON_STATE_INACTIVE 2

class CPButton : public CGameObject {
private:
	bool isHit = false;
	float puffTimer = 0.0f;

	static bool IsContentLoaded;
	static void LoadContent();
public:

	CPButton(float x, float y);
	void Render() override;
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	
	int IsCollidable() override { return !isDeleted && (state != P_BUTTON_STATE_INACTIVE); }
	int IsBlocking() override { return !isDeleted && (state != P_BUTTON_STATE_INACTIVE); }
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