#include "LevelEndReward.h"
#include "Engine/Loaders/SpritesLoader.h"
#include "ContentIds/LevelEnd.h"
#include <Engine/Graphics/Sprites.h>
#include "GameObjects/Mario.h"
#include "FireworkPattern.h"

bool CLevelEndReward::IsContentLoaded = false;
void CLevelEndReward::LoadContent()
{
	if (IsContentLoaded)
		return;
	SpritesLoader loader;
	loader.Load(LEVEL_END_SPRITES_PATH);
}

CLevelEndReward::CLevelEndReward(float x, float y, int animationID)
	: CGameObject(x, y, 0.0f), animationID(animationID)
{
	layer = SortingLayer::CORPSE;
	LoadContent();
	SetState(LEVEL_END_REWARD_STATE_NORMAL);
}

void CLevelEndReward::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (timer > 0) timer -= dt;
	else {
		if (state == LEVEL_END_REWARD_STATE_END) {
			Delete();
		}
		if (state == LEVEL_END_REWARD_STATE_NORMAL) {
			SetState(LEVEL_END_REWARD_STATE_END);
		}
	}
	if (state == LEVEL_END_REWARD_STATE_NORMAL) {
		if (firework)
			position.x -= 0x000A0 * SUBSUBSUBPIXEL_DELTA_TIME * dt;
		position.y -= 0x02800 * SUBSUBSUBPIXEL_DELTA_TIME * dt;
	}
}

void CLevelEndReward::Render()
{
	CAnimations* const animations = CAnimations::GetInstance();
	if (state == LEVEL_END_REWARD_STATE_NORMAL)
	    animations->Get(animationID)->Render(position.x, position.y, 1.0f);
	if (state == LEVEL_END_REWARD_STATE_END)
		animations->Get(LEVEL_END_ID_ANIMATION_END)->Render(position.x, position.y, 1.0f);
	
}

void CLevelEndReward::SetState(int state)
{
	switch (state) {
	case LEVEL_END_REWARD_STATE_NORMAL:
		timer = LEVEL_END_REWARD_NORMAL_TIMER;
		break;
	case LEVEL_END_REWARD_STATE_END: 
	{
		if (firework) {
			if (animationID == LEVEL_END_ID_ANIMATION_MUSHROOM)
				SpawnFireworkPattern(0, position);
			else if (animationID == LEVEL_END_ID_ANIMATION_FLOWER)
				SpawnFireworkPattern(1, position);
			else
				SpawnFireworkPattern(2, position);
		}
		timer = LEVEL_END_REWARD_END_TIMER;
		CAnimations* const animations = CAnimations::GetInstance();
		auto animation = animations->GetInstance()->Get(LEVEL_END_ID_ANIMATION_END);
		animation->SetLoop(false);
		animation->Reset();
	}
		break;
	}
	this->state = state;
}

void CLevelEndReward::OnDelete()
{
}
