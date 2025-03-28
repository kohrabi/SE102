#include "CTankEnemyGreen.h"
#include "Engine/Game.h"
#include "CTankEnemyRed.h"

void CTankEnemyGreen::OnDestroy()
{
	CGame* const game = CGame::GetInstance();
	game->objects.push_back(new CTankEnemyRed(position.x, position.y, 0.f, timeOffset));
}
