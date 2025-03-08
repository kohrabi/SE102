#pragma once


#include "GameObject.h"

struct AABB {
	float left;
	float right;
	float top;
	float bottom;
};

class CTank : public CMoveableObject {
protected:
	RECT UpTextureRect = { 0 };
	RECT RightTextureRect = { 0 };
	RECT LeftTextureRect = { 0 };
	RECT DownTextureRect = { 0 };
	Vector2 lookDir = Vector2(0, -1);
	Vector2 moveDir = Vector2::Zero;
	CTankBullet* tankBullet;
	bool fire = false;
	bool canFire = true;
	AABB collisionBox;
	bool isPlayer = false;
public:
	CTank(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture) : CMoveableObject(x, y, rotation, vx, vy, texture) {
		collisionBox.left = 0;
		collisionBox.right = 16;
		collisionBox.top = 0;
		collisionBox.bottom = 16;
	}
	int GetType() const override { return TYPE_TANK; }
	bool IsPlayer() const { return isPlayer; }

	bool checkPointInside(Vector2 point) const;
	
	void Update(DWORD dt) override;
};