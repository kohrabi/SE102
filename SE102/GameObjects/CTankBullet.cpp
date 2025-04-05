#include "CTankBullet.h"
#include "Engine/Graphics/Sprites.h""
#include "contents.h"
#include "CTankEnemy.h"
#include <Engine/debug.h>
#include "CTile.h"

void CTankBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted)
		return;
	CGame* const game = CGame::GetInstance();
	auto texture = CSprites::GetInstance()->Get(TANK_BULLET_SPRITE_ID);
	
	int backBufferHeight = game->GetBackBufferHeight();
	int imageHeight = texture->getWidth();
	imageHeight /= 2;
	if (position.y <= 0 || position.y >= backBufferHeight - imageHeight) {
		isDeleted = true;
	}

	int backBufferWidth = game->GetBackBufferWidth();
	int imageWidth = texture->getWidth();
	imageWidth /= 2;
	if (position.x <= 0 || position.x >= backBufferWidth - imageWidth) {
		isDeleted = true;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTankBullet::Render()
{
	CSprites* const sprites = CSprites::GetInstance();

	sprites->Get(TANK_BULLET_SPRITE_ID)->Draw(position.x, position.y);
}

void CTankBullet::OnNoCollision(DWORD dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
}

void CTankBullet::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CTile*>(e->obj)) {
		Delete();
		e->obj->Delete();
	}
	if (isPlayer && dynamic_cast<CTankEnemy*>(e->obj)) {
		Delete();
		e->obj->Delete();
	}
}
