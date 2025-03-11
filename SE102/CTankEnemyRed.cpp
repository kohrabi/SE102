#include "CTankEnemyRed.h"
#include "Game.h"
#include "debug.h"

void CTankEnemyRed::OnDestroy()
{
	CGame* const game = CGame::GetInstance();
	game->objects.push_back(new CTankEnemy(position.x, position.y, 0.f, 0.f, 0.f, game->texBTSprites, timeOffset));
}
