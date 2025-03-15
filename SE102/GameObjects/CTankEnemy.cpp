#include "CTankEnemy.h"


void CTankEnemy::Update(DWORD dt)
{
	float dts = dt / 1000.0f;
	if (moveChangeDirTimer > 0) moveChangeDirTimer -= dts;
	else {
		moveChangeDirTimer = MOVE_DIR_CHANGE_TIME + timeOffset;
		int dir = rand() % 4;
		switch (dir) {
		case 0:
			moveDir.x = 1;
			moveDir.y = 0;
			break;
		case 1:
			moveDir.x = -1;
			moveDir.y = 0;
			break;
		case 2:
			moveDir.x = 0;
			moveDir.y = 1;
			break;
		case 3:
			moveDir.x = 0;
			moveDir.y = -1;
			break;
		}
	}

	fire = false;
	if (canFire) {
		if (shootTimer > 0.0f) shootTimer -= dts;
		if (shootTimer <= 0.0f) {
			shootTimer = SHOOT_TIME;
			fire = true;
		}
	}

	CTank::Update(dt);
}

void CTankEnemy::Render()
{
	CTank::Render();
}
