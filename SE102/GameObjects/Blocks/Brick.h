#pragma once

#include "Engine/GameObject.h"
#include "Engine/debug.h"

#include "Engine/CollisionCast.h"

#define BRICK_STATE_NORMAL 1
#define BRICK_STATE_BREAK 2

class CBrick : public CGameObject {
private:
	bool isHit = false;
	CCollisionCast turnCast;
	Vector2 debrisPos[4];
	Vector2 debrisVel[4];

	static bool IsContentLoaded;
	static void LoadContent();
public:

	CBrick(float x, float y);
	void Render() override;
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	
	int IsCollidable() override { return !isDeleted && (state != BRICK_STATE_BREAK); }
	int IsBlocking() override { return !isDeleted && (state != BRICK_STATE_BREAK); }
	void SetState(int state) override;
	void Hit();
	void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		const float size = 16.0f / 2.0f;
		left = position.x - size;
		top = position.y - size;
		right = position.x + size;
		bottom = position.y + size;
	}
};