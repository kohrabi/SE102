#include "CTank.h"
#include "Game.h"

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

void CTank::Update(DWORD dt)
{
	if (destroy)
		return;
	float dts = dt / 1000.f;
	CGame* game = CGame::GetInstance();

	if (abs(moveDir.x) == 1 && abs(moveDir.y) == 1) {
		moveDir = Vector2::Zero;
	}

	if (moveDir != Vector2::Zero) {
		lookDir = moveDir;
	}

	velocity.x = moveDir.x * TANK_SPEED * dts;
	velocity.y = moveDir.y * TANK_SPEED * dts;
		

	if (tankBullet != NULL) {
		if (tankBullet->ShouldDestroy())
			canFire = true;
	}

	if (fire && canFire) {
		tankBullet = new CTankBullet(position.x, position.y, 0.f, BULLET_SPEED * lookDir.x, BULLET_SPEED * lookDir.y, game->texBTSprites, isPlayer);
		game->objects.push_back(tankBullet);
		fire = false;
		canFire = false;
	}

	if (moveDir.x == 1) {
		textureRegion = RightTextureRect;
	}
	else if (moveDir.x == -1) {
		textureRegion = LeftTextureRect;
	}
	else if (moveDir.y == -1) {
		textureRegion = UpTextureRect;
	}
	else if (moveDir.y == 1) {
		textureRegion = DownTextureRect;
	}


	int BackBufferHeight = game->GetViewportHeight();
	int BackBufferWidth = game->GetViewportWidth();
	int imageHeight = texture->getHeight();
	if (textureRegion.bottom - textureRegion.top != 0)
		imageHeight = textureRegion.bottom - textureRegion.top;

	int imageWidth = texture->getWidth();
	
	if (textureRegion.right - textureRegion.left != 0)
		imageWidth = textureRegion.right - textureRegion.left;

	CMoveableObject::Update(dt);

	if (position.y <= imageHeight / 2 || position.y >= BackBufferHeight - imageHeight) {
		if (position.y <= imageHeight / 2)
			position.y = imageHeight / 2;
		else if (position.y >= BackBufferHeight - imageHeight)
			position.y = (float)(BackBufferHeight - imageHeight);
	}
	if (position.x <= imageWidth / 2 || position.x >= BackBufferWidth - imageWidth) {
		if (position.x <= imageWidth / 2)
			position.x = imageWidth / 2;
		else if (position.x >= BackBufferWidth - imageWidth)
			position.x = (float)(BackBufferWidth - imageWidth);
	}

}
