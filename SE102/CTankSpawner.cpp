#include "CTankSpawner.h"
#include "Game.h"
#include "CTankEnemy.h"

void CTankSpawner::Update(DWORD dt)
{
	CGame* const game = CGame::GetInstance();
	float dts = dt / 1000.f;
	if (spawnTimer > 0) spawnTimer -= dts;
	else {
		spawnTimer = spawnTime;
		game->objects.push_back(new CTankEnemy(position.x, position.y, 0.0f, 0.0f, 0.0f, game->texBTSprites, SHOOT_TIME));
	}
}
