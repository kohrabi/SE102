#pragma once

#include "Engine/GameObject.h"

class CTankBullet : public CGameObject {
private:
	bool isPlayer = false;
public:
	CTankBullet(float x, float y, float rotation, bool player = false) : CGameObject(x, y, rotation)
	{
		isPlayer = player;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	int IsBlocking() override { return 0; }
	int IsCollidable() override { return 1; }
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		left = position.x - 2;
		top = position.y - 2;
		right = position.x + 2;
		bottom = position.y + 2;
	}
};