#include "CTankBullet.h"
#include "Engine/Graphics/Sprites.h""
#include "contents.h"
#include "CTankEnemy.h"
#include <Engine/debug.h>

void CTankBullet::Update(DWORD dt)
{
	if (destroy)
		return;
	CGame* const game = CGame::GetInstance();
	auto texture = CSprites::GetInstance()->Get(TANK_BULLET_SPRITE_ID);
	
	int backBufferHeight = game->GetViewportHeight();
	int imageHeight = texture->getWidth();
	imageHeight /= 2;
	if (position.y <= 0 || position.y >= backBufferHeight - imageHeight) {
		destroy = true;
	}

	int backBufferWidth = game->GetViewportWidth();
	int imageWidth = texture->getWidth();
	imageWidth /= 2;
	if (position.x <= 0 || position.x >= backBufferWidth - imageWidth) {
		destroy = true;
	}

	if (!destroy) {
		size_t size = game->objects.size();
		for (int i = 0; i < size; i++) {
			auto objectA = game->objects[i];
			if (objectA->ShouldDestroy() || objectA == this)
				continue;
			if (objectA->GetType() == TYPE_ENEMY) {
				auto tank = (CTankEnemy*)objectA;
				if (tank->IsPlayer() == isPlayer)
					continue;
				if (tank->checkPointInside(position)) {
					tank->Destroy();
					Destroy();
					break;
				}
			}
			if (objectA->GetType() == TYPE_BULLET && (objectA->GetPosition() - position).length() <= 3.0f)
			{
				DebugOut(L"%f\n", (objectA->GetPosition() - position).length());
				Destroy();
				objectA->Destroy();
				break;
			}
		}
	}

	CMoveableObject::Update(dt);
}

void CTankBullet::Render()
{
	CSprites* const sprites = CSprites::GetInstance();

	sprites->Get(TANK_BULLET_SPRITE_ID)->Draw(position.x, position.y);
}
