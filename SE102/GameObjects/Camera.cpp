#include "Camera.h"
#include "Engine/Game.h"

#include "Engine/PlayScene.h"
#include "Mario.h"
#include <Engine/Helper.h>
#include <iostream>

using namespace std;

void CCamera::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* const game = CGame::GetInstance();
	CPlayScene* const scene = dynamic_cast<CPlayScene*>(game->GetCurrentScene());
	ASSERT(scene != NULL, "This is illegal");

	CMario* player = dynamic_cast<CMario*>(scene->GetPlayer());

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	AABB levelBound = scene->GetLevelBound();
	AABB playerAABB;
	if (scene->GetPlayer()->GetState() != MARIO_STATE_DEAD)
	{
		player->GetBoundingBox(playerAABB.left, playerAABB.top, playerAABB.right, playerAABB.bottom);
		if (!CCollision::CheckAABBOverlaps(levelBound, playerAABB))
		{
			for (auto bound : scene->GetLevelBounds())
			{
				if (CCollision::CheckAABBOverlaps(bound, playerAABB))
				{
					levelBound = bound;
					break;
				}
			}
		}
	}

	if (player->GetState() != MARIO_STATE_DEAD)
	{
		// Update camera to follow mario
		Vector2 playerPos = player->GetPosition();
		Vector2 camPos = Vector2::Zero;
		game->GetCamPos(camPos.x, camPos.y);
		Vector2 camCenter = camPos + Vector2(game->GetBackBufferWidth(), game->GetBackBufferHeight()) / 2.0f;
		if (abs(camCenter.x - playerPos.x) > DEADZONE_X)
			position.x = playerPos.x + round(deadzoneOffset.x);
		else
			deadzoneOffset.x = camCenter.x - playerPos.x;
		if (player->IsOnGround() && abs(deadzoneOffset.y) > 0)
			deadzoneOffset.y = round(deadzoneOffset.y + -0x00A00 * SUBSUBSUBPIXEL_DELTA_TIME * dt * sign(deadzoneOffset.y));

		if (abs(playerPos.y - camCenter.y) > DEADZONE_Y)
			position.y = playerPos.y + round(deadzoneOffset.y);
		else
			deadzoneOffset.y = -(playerPos.y - camCenter.y);

	}

	Vector2 setPos = position;
	setPos.x -= game->GetBackBufferWidth() / 2;
	setPos.y -= game->GetBackBufferHeight() / 2;
	setPos.x = clampf(setPos.x, levelBound.left, levelBound.right - game->GetBackBufferWidth());
	setPos.y = clampf(setPos.y, levelBound.top, levelBound.bottom - game->GetBackBufferHeight());
	if (shakeTimer > 0)
	{
		if (shakeFrameTimer > 0) shakeFrameTimer -= dt;
		else
		{
			setPos.y += nx * 2.0f;
			nx *= -1;
		}
		shakeTimer -= dt;
	}

	game->SetCamPos(round(setPos.x), round(setPos.y) /*cy*/);
}

void CCamera::Render()
{
}
