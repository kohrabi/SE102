#include "CTankEnemyRed.h"
#include "Engine/Game.h"
#include "Engine/debug.h"

void CTankEnemyRed::OnDestroy()
{
	CGame* const game = CGame::GetInstance();
	game->objects.push_back(new CTankEnemy(position.x, position.y, 0.f, timeOffset));
}
