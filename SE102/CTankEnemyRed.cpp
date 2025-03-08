#include "CTankEnemyRed.h"
#include "Game.h"
#include "debug.h"

void CTankEnemyRed::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	DebugOut(L"%d", destroy);
	if (destroy && !spawnNext)
	{
		spawnNext = true;
		game->objects.push_back(new CTankEnemy(position.x, position.y, 0.f, 0.f, 0.f, game->texBTSprites, timeOffset));
	}

	CTankEnemy::Update(dt);
}
