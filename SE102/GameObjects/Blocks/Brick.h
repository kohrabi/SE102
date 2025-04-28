#pragma once

#include "Engine/GameObject.h"
#include "Engine/debug.h"

#include "Engine/CollisionCast.h"

#define QUESTION_BLOCK_SPAWN_COIN 1
#define QUESTION_BLOCK_SPAWN_LEAF 2
#define QUESTION_BLOCK_SPAWN_ONE_UP 3
#define QUESTION_BLOCK_ANIMATION_TIME 100
#define QUESTION_BLOCK_ANIMATION_Y_VEL 0.1f

class CBrick : public CGameObject {
private:
	bool isHit = false;
	CCollisionCast holdCast;
	
	static bool IsContentLoaded;
	static void LoadContent();
public:

	CBrick(float x, float y);
	void Render() override;
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	void Hit(int dx);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		const float size = 16.0f / 2.0f;
		left = position.x - size;
		top = position.y - size;
		right = position.x + size;
		bottom = position.y + size;
	}
};