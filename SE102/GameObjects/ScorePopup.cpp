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
	int animationId = UI_ID_SPRITE_100;
	switch (scoreType)
	{
	case Score100: animationId = UI_ID_SPRITE_100; break;
	case Score200: animationId = UI_ID_SPRITE_200; break;
	case Score400: animationId = UI_ID_SPRITE_400; break;
	case Score800: animationId = UI_ID_SPRITE_800; break;
	case Score1000: animationId = UI_ID_SPRITE_1000; break;
	case Score2000: animationId = UI_ID_SPRITE_2000; break;
	case Score4000: animationId = UI_ID_SPRITE_4000; break;
	case Score8000: animationId = UI_ID_SPRITE_8000; break;
	}
	sprites->Get(animationId)->Draw(position.x, position.y, GetLayer(layer, 0));
}
