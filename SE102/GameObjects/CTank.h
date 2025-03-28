#pragma once


#include "Engine/GameObject.h"
#include "Engine/Graphics/Animation.h"
#include "Engine/Graphics/Animations.h"
#include "Engine/Math/AABB.h"
#include "CTankBullet.h"
#include "contents.h"

class CTank : public CGameObject {
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
	CTank(float x, float y, float rotation) : CGameObject(x, y, rotation) {
		collisionBox.left = 0;
		collisionBox.right = 16;
		collisionBox.top = 0;
		collisionBox.bottom = 16;
	}
	bool IsPlayer() const { return isPlayer; }

	bool checkPointInside(Vector2 point) const;
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;

	int IsCollidable() override { return 1; }
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e);
	int IsBlocking() override { return 0; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		left =  position.x - 8;
		top = position.y - 8;
		right = position.x + 8;
		bottom = position.y + 8;
	}
};