#include "CPlayer.h"
#include "Game.h"

void CPlayer::Update(DWORD dt)
{
	CGame* const game = CGame::GetInstance();

	Vector2 inputDir = Vector2();
	if (game->keyState[VK_LEFT])
		inputDir.x += -1;
	if (game->keyState[VK_RIGHT])
		inputDir.x += 1;

	if (game->keyState[VK_UP])
		inputDir.y += -1;
	if (game->keyState[VK_DOWN])
		inputDir.y += 1;

	if (abs(inputDir.x) == 1 && abs(inputDir.y) == 1) {
		inputDir = Vector2::Zero;
	}

	if (inputDir != Vector2::Zero) {
		lookDir = inputDir;
	}

	moveDir.x = inputDir.x;
	moveDir.y = inputDir.y;

	fire = false;
	if (game->keyJustPressed[VK_SPACE])
		fire = true;

	CTank::Update(dt);
}
