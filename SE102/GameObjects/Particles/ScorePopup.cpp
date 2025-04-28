#include "ScorePopup.h"
#include "Engine/Loaders/SpritesLoader.h"
#include "ContentIds/UI.h"
#include <Engine/Graphics/Sprites.h>
#include "GameObjects/Mario.h"

bool CScorePopup::IsContentLoaded = false;
void CScorePopup::LoadContent()
{
	if (IsContentLoaded)
		return;
	SpritesLoader loader;
	loader.Load(UI_SPRITES_PATH);
}

CScorePopup::CScorePopup(float x, float y, ScoreType scoreType)
	: CGameObject(x, y, 0.0f)
{
	velocity.y = -SCORE_POPUP_RISE_VELOCITY;
	layer = SortingLayer::CORPSE;
	this->scoreType = scoreType;
	LPSCENE currentScene = CGame::GetInstance()->GetCurrentScene();
	if (currentScene != NULL)
	{
		CMario* mario = dynamic_cast<CMario*>(currentScene->GetPlayer());
		if (mario != NULL)
		{
			switch (scoreType)
			{
			case Score100: mario->AddScore(100); break;
			case Score200: mario->AddScore(200); break;
			case Score400: mario->AddScore(400); break;
			case Score800: mario->AddScore(800); break;
			case Score1000: mario->AddScore(1000); break;
			case Score2000: mario->AddScore(2000); break;
			case Score4000: mario->AddScore(4000); break;
			case Score8000: mario->AddScore(8000); break;
			}
		}
	}
	LoadContent();
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
	case OneUp: animationId = UI_ID_SPRITE_ONEUP; break;
	}
	sprites->Get(animationId)->Draw(position.x, position.y, GetLayer(layer, 0));
}
