#include "ScorePopup.h"
#include "Engine/Loaders/SpritesLoader.h"
#include "ContentIds/UI.h"
#include <Engine/Graphics/Sprites.h>

bool CScorePopup::IsContentLoaded = false;
void CScorePopup::LoadContent()
{
	if (IsContentLoaded)
		return;
	SpritesLoader loader;
	loader.Load(UI_SPRITES_PATH);
}

void CScorePopup::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
	velocity.y = min(velocity.y + SCORE_POPUP_GRAVITY, 0.0f);
	position.y += velocity.y * dt;
	if (destroyTimer <= 0)
		Delete();
	destroyTimer -= dt;
}

void CScorePopup::Render()
{
	CSprites* const sprites = CSprites::GetInstance();
	sprites->Get(UI_ID_SPRITE_100)->Draw(position.x, position.y, GetLayer(layer, 0));
}
