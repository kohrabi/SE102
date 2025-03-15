#pragma once


#include "Engine/GameObject.h"
#include "Engine/Graphics/Animation.h"
#include "Engine/Graphics/Animations.h"
#include "Engine/Math/AABB.h"
#include "CTankBullet.h"
#include "contents.h"

class CTank : public CMoveableObject {
protected:
	LPANIMATION currentAnimation = NULL;
	LPANIMATION MoveUpAnimation = NULL;
	LPANIMATION MoveRightAnimation = NULL;
	LPANIMATION MoveLeftAnimation = NULL;
	LPANIMATION MoveDownAnimation = NULL;
	Vector2 lookDir = Vector2(0, -1);
	Vector2 moveDir = Vector2::Zero;
	CTankBullet* tankBullet = NULL;
	bool fire = false;
	bool canFire = true;
	AABB collisionBox;
	bool isPlayer = false;
public:
	CTank(float x, float y, float rotation, float vx, float vy) : CMoveableObject(x, y, rotation, vx, vy) {
		collisionBox.left = 0;
		collisionBox.right = 16;
		collisionBox.top = 0;
		collisionBox.bottom = 16;
	}
	int GetType() const override { return TYPE_TANK; }
	bool IsPlayer() const { return isPlayer; }

	bool checkPointInside(Vector2 point) const;
	
	void Update(DWORD dt) override;
	void Render() override;
};