#include "CPlayer.h"
#include "Engine/Game.h"
#include "Engine/Graphics/Textures.h"

void CPlayer::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* const game = CGame::GetInstance();

	Vector2 inputDir = Vector2();
	if (game->IsKeyDown(VK_LEFT))
		inputDir.x += -1;
	if (game->IsKeyDown(VK_RIGHT))
		inputDir.x += 1;

	if (game->IsKeyDown(VK_UP))
		inputDir.y += -1;
	if (game->IsKeyDown(VK_DOWN))
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
	if (game->IsKeyJustPressed(VK_SPACE))
		fire = true;

	CTank::Update(dt, coObjects);
}

void CPlayer::Render()
{
	CTank::Render();
}

void CPlayer::LoadContent()
{
	CTextures* const textures = CTextures::GetInstance();
	CSprites* const sprites = CSprites::GetInstance();
	CAnimations* const animations = CAnimations::GetInstance();

	auto texBTSprites = textures->Get(TEXTURE_PATH_BTSPRITES);
	sprites->Add(TANK_BULLET_SPRITE_ID, 322, 102, 325, 105, texBTSprites);
	sprites->Add(PLAYER_MOVEUP_FRAME1, GetTextureRegion(0, 0, 16, 16), texBTSprites);
	sprites->Add(PLAYER_MOVEUP_FRAME2, GetTextureRegion(1, 0, 16, 16), texBTSprites);

	CAnimation* ani = new CAnimation(200);
	ani->Add(PLAYER_MOVEUP_FRAME1);
	ani->Add(PLAYER_MOVEUP_FRAME2);
	animations->Add(PLAYER_MOVEUP_ANIMATION, ani);

	sprites->Add(PLAYER_MOVELEFT_FRAME1, GetTextureRegion(2, 0, 16, 16), texBTSprites);
	sprites->Add(PLAYER_MOVELEFT_FRAME2, GetTextureRegion(3, 0, 16, 16), texBTSprites);

	ani = new CAnimation(200);
	ani->Add(PLAYER_MOVELEFT_FRAME1);
	ani->Add(PLAYER_MOVELEFT_FRAME2);
	animations->Add(PLAYER_MOVELEFT_ANIMATION, ani);

	sprites->Add(PLAYER_MOVEDOWN_FRAME1, GetTextureRegion(4, 0, 16, 16), texBTSprites);
	sprites->Add(PLAYER_MOVEDOWN_FRAME2, GetTextureRegion(5, 0, 16, 16), texBTSprites);

	ani = new CAnimation(200);
	ani->Add(PLAYER_MOVEDOWN_FRAME1);
	ani->Add(PLAYER_MOVEDOWN_FRAME2);
	animations->Add(PLAYER_MOVEDOWN_ANIMATION, ani);

	sprites->Add(PLAYER_MOVERIGHT_FRAME1, GetTextureRegion(6, 0, 16, 16), texBTSprites);
	sprites->Add(PLAYER_MOVERIGHT_FRAME2, GetTextureRegion(7, 0, 16, 16), texBTSprites);

	ani = new CAnimation(200);
	ani->Add(PLAYER_MOVERIGHT_FRAME1);
	ani->Add(PLAYER_MOVERIGHT_FRAME2);
	animations->Add(PLAYER_MOVERIGHT_ANIMATION, ani);
}
