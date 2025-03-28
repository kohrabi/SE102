#include "CTankEnemy.h"
#include <Engine/Graphics/Textures.h>


void CTankEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

	CTank::Update(dt, coObjects);
}

void CTankEnemy::Render()
{
	CTank::Render();
}

void CTankEnemy::LoadContent()
{
	CTextures* const textures = CTextures::GetInstance();
	CSprites* const sprites = CSprites::GetInstance();
	CAnimations* const animations = CAnimations::GetInstance();

	auto texBTSprites = textures->Get(TEXTURE_PATH_BTSPRITES);

	sprites->Add(ENEMY_MOVEUP_FRAME1, GetTextureRegion(0 + 8, 0, 16, 16), texBTSprites);
	sprites->Add(ENEMY_MOVEUP_FRAME2, GetTextureRegion(1 + 8, 0, 16, 16), texBTSprites);

	CAnimation* ani = new CAnimation(200);
	ani->Add(ENEMY_MOVEUP_FRAME1);
	ani->Add(ENEMY_MOVEUP_FRAME2);
	animations->Add(ENEMY_MOVEUP_ANIMATION, ani);

	sprites->Add(ENEMY_MOVELEFT_FRAME1, GetTextureRegion(2 + 8, 0, 16, 16), texBTSprites);
	sprites->Add(ENEMY_MOVELEFT_FRAME2, GetTextureRegion(3 + 8, 0, 16, 16), texBTSprites);

	ani = new CAnimation(200);
	ani->Add(ENEMY_MOVELEFT_FRAME1);
	ani->Add(ENEMY_MOVELEFT_FRAME2);
	animations->Add(ENEMY_MOVELEFT_ANIMATION, ani);

	sprites->Add(ENEMY_MOVEDOWN_FRAME1, GetTextureRegion(4 + 8, 0, 16, 16), texBTSprites);
	sprites->Add(ENEMY_MOVEDOWN_FRAME2, GetTextureRegion(5 + 8, 0, 16, 16), texBTSprites);

	ani = new CAnimation(200);
	ani->Add(ENEMY_MOVEDOWN_FRAME1);
	ani->Add(ENEMY_MOVEDOWN_FRAME2);
	animations->Add(ENEMY_MOVEDOWN_ANIMATION, ani);

	sprites->Add(ENEMY_MOVERIGHT_FRAME1, GetTextureRegion(6 + 8, 0, 16, 16), texBTSprites);
	sprites->Add(ENEMY_MOVERIGHT_FRAME2, GetTextureRegion(7 + 8, 0, 16, 16), texBTSprites);

	ani = new CAnimation(200);
	ani->Add(ENEMY_MOVERIGHT_FRAME1);
	ani->Add(ENEMY_MOVERIGHT_FRAME2);
	animations->Add(ENEMY_MOVERIGHT_ANIMATION, ani);
}
