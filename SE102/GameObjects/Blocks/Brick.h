#pragma once

#include "Engine/GameObject.h"
#include "Engine/debug.h"

#include "Engine/CollisionCast.h"

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