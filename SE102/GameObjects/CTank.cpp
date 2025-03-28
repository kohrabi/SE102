#include "CTank.h"
#include "Engine/Game.h"
#include <Engine/debug.h>
#include "CTile.h"

#define TANK_SPEED 1.4f
#define BULLET_SPEED 0.2f

bool CTank::checkPointInside(Vector2 point) const
{
	AABB newBox;
	float sizeX = abs(collisionBox.right - collisionBox.left) / 2.f;
	float sizeY = abs(collisionBox.top - collisionBox.bottom) / 2.f;
	newBox.left = position.x - sizeX;
	newBox.right = position.x + sizeX;
	newBox.top = position.y - sizeY;
	newBox.bottom = position.y + sizeY;
	return (point.x <= newBox.right && point.x >= newBox.left) && (point.y <= newBox.bottom && point.y >= newBox.top);
}



void CTank::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (destroy)
		return;
	float dts = dt / 1000.f;
	CGame* const game = CGame::GetInstance();

	if (abs(moveDir.x) == 1 && abs(moveDir.y) == 1) {
		moveDir = Vector2::Zero;
	}

	if (moveDir != Vector2::Zero) {
		lookDir = moveDir;
	}

	velocity.x = moveDir.x * TANK_SPEED * dts;
	velocity.y = moveDir.y * TANK_SPEED * dts;

	if (tankBullet != NULL) {
		if (tankBullet->IsDestroyed())
			canFire = true;
	}

	if (fire && canFire) {
		tankBullet = new CTankBullet(position.x, position.y, 0.f, isPlayer);
		tankBullet->SetVelocity(BULLET_SPEED * lookDir.x, BULLET_SPEED * lookDir.y);
		game->objects.push_back(tankBullet);
		fire = false;
		canFire = false;
	}

	if (currentAnimation != NULL) {
		if (velocity.lengthSquared() > 0)
			currentAnimation->Play();
		else
			currentAnimation->Stop();
	}

	int BackBufferHeight = game->GetViewportHeight();
	int BackBufferWidth = game->GetViewportWidth();

	int imageHeight = 16;
	imageHeight /= 2;

	int imageWidth = 16;
	imageWidth /= 2;

	CCollision::GetInstance()->Process(this, dt, coObjects);

	if (position.y <= imageHeight || position.y >= BackBufferHeight - imageHeight) {
		if (position.y <= imageHeight)
			position.y = imageHeight;
		else if (position.y >= BackBufferHeight - imageHeight)
			position.y = (float)(BackBufferHeight - imageHeight);
	}
	if (position.x <= imageWidth || position.x >= BackBufferWidth - imageWidth) {
		if (position.x <= imageWidth)
			position.x = imageWidth;
		else if (position.x >= BackBufferWidth - imageWidth)
			position.x = (float)(BackBufferWidth - imageWidth);
	}

}

void CTank::Render()
{
	if (lookDir.x == 1) {
		currentAnimation = MoveRightAnimation;
	}
	else if (lookDir.x == -1) {
		currentAnimation = MoveLeftAnimation;
	}
	else if (lookDir.y == -1) {
		currentAnimation = MoveUpAnimation;
	}
	else if (lookDir.y == 1) {
		currentAnimation = MoveDownAnimation;
	}

	currentAnimation->Render(position.x, position.y);
}

void CTank::OnNoCollision(DWORD dt)
{
	float dts = dt / 1000.f;
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
}

void CTank::OnCollisionWith(LPCOLLISIONEVENT e)
{
}
