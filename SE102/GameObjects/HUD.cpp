#include "HUD.h"
#include <Engine/Loaders/SpritesLoader.h>
#include <ContentIds/HUD.h>

#include "Engine/debug.h"

#include "Engine/Game.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "string"
#include "Mario.h"

bool CHUD::IsContentLoaded = false;
void CHUD::LoadContent()
{
	if (IsContentLoaded)
		return;
	SpritesLoader loader;
	loader.Load(HUD_SPRITES_PATH);
}

int CHUD::getNumberSpriteId(char number)
{
	switch (number)
	{
	case '0': return HUD_ID_SPRITE_CHARACTERS_FRAME_0;
	case '1': return HUD_ID_SPRITE_CHARACTERS_FRAME_1;
	case '2': return HUD_ID_SPRITE_CHARACTERS_FRAME_2;
	case '3': return HUD_ID_SPRITE_CHARACTERS_FRAME_3;
	case '4': return HUD_ID_SPRITE_CHARACTERS_FRAME_4;
	case '5': return HUD_ID_SPRITE_CHARACTERS_FRAME_5;
	case '6': return HUD_ID_SPRITE_CHARACTERS_FRAME_6;
	case '7': return HUD_ID_SPRITE_CHARACTERS_FRAME_7;
	case '8': return HUD_ID_SPRITE_CHARACTERS_FRAME_8;
	case '9': return HUD_ID_SPRITE_CHARACTERS_FRAME_9;
	}
	ASSERT(false, "Unavailable number");
	return 0;
}

void CHUD::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CHUD::Render()
{
	if (currentScene == NULL)
		return;
	CGame* const game = CGame::GetInstance();
	CSprites* const sprites = CSprites::GetInstance();
	CAnimations* const animations = CAnimations::GetInstance();
	LPSPRITE mainFrame = sprites->Get(HUD_ID_SPRITE_MAIN_FRAME);
	mainFrame->DrawScreen(mainFrame->getWidth() / 2.0f - 8.0f, game->GetBackBufferHeight() - 12.0f, 1.0f);

	// Life
	CMario* const mario = dynamic_cast<CMario*>(currentScene->GetPlayer());
	LPSPRITE marioLife = sprites->Get(HUD_ID_SPRITE_MARIO_LIFE);
	marioLife->DrawScreen(16.0f, game->GetBackBufferHeight() - 15.0f, 1.0f);
	
	string life = to_string(min(currentScene->GetMarioLife(), 99));
	for (int i = 0; i < life.size(); i++)
	{
		sprites->Get(getNumberSpriteId(life[i]))->DrawScreen(44.0f - i * 8.0f, game->GetBackBufferHeight() - 15.0f, 1.0f);
	}

	// POWER
	int powerCount = mario->GetPowerCount();
	for (int i = 0; i < 6; i++)
	{
		int arrowFrame = HUD_ID_SPRITE_ARROW_FRAME_1;
		if (i + 1 <= powerCount)
			arrowFrame = HUD_ID_SPRITE_ARROW_FRAME_0;
		sprites->Get(arrowFrame)->DrawScreen(60.0f + i * 8.0f, game->GetBackBufferHeight() - 23.0f, 1.0f);
	}
	
	if (powerCount < MAX_POWER_COUNT)
	{
		LPSPRITE power = sprites->Get(HUD_ID_SPRITE_POWER_FRAME_1);
		power->DrawScreen(112.0f, game->GetBackBufferHeight() - 23.0f, 1.0f);
	}
	else
	{
		animations->Get(HUD_ID_ANIMATION_POWER)->RenderScreen(112.0f, game->GetBackBufferHeight() - 23.0f, 1.0f);
	}

	// Score
	string score = to_string(mario->GetScore());
	while (score.size() < 7)
		score.insert(score.begin(), '0');
	for (int i = 0; i < 7; i++)
	{
		sprites->Get(getNumberSpriteId(score[i]))->DrawScreen(60.0f + i * 8.0f, game->GetBackBufferHeight() - 15.0f, 1.0f);
	}

	// Timer
	string currentTime = to_string((int)(currentScene->GetLevelTime() / 1000.0f));
	for (int i = 0; i < currentTime.size(); i++)
	{
		sprites->Get(getNumberSpriteId(currentTime[i]))->DrawScreen(132.0f + i * 8.0f, game->GetBackBufferHeight() - 15.0f, 1.0f);
	}

	// Money counter
	int coinCount = 0;
	if (mario != NULL)
		coinCount = min(mario->GetCoinCount(), 99);
	string coin = to_string(coinCount);
	for (int i = 0; i < coin.size(); i++)
	{
		sprites->Get(getNumberSpriteId(coin[coin.size() - i - 1]))->DrawScreen(148.0f - i * 8.0f, game->GetBackBufferHeight() - 23.0f, 1.0f);
	}

	// World count
	sprites->Get(getNumberSpriteId('1'))->DrawScreen(44.0f, game->GetBackBufferHeight() - 23.0f, 1.0f);

}