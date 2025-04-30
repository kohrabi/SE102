#include "LevelEnd.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/LevelEnd.h"

#include <iostream>
using namespace std;

bool CLevelEnd::IsContentLoaded = false;

void CLevelEnd::LoadContent()
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(LEVEL_END_SPRITES_PATH);
}


CLevelEnd::CLevelEnd(float x, float y)
    : CGameObject(x, y, 0.0f)
{
    LoadContent();
    layer = SortingLayer::NPC;
    CAnimations::GetInstance()->Get(LEVEL_END_ID_ANIMATION_SLOT)->Play();
    SetState(LEVEL_END_STATE_NORMAL);
}

void CLevelEnd::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (state == LEVEL_END_STATE_EMPTY)
        yOffset -= 0x02800 * SUBSUBSUBPIXEL_DELTA_TIME * dt;
}

void CLevelEnd::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    CSprites* const sprites = CSprites::GetInstance();

    sprites->Get(LEVEL_END_ID_SPRITE_BOX)->Draw(position.x, position.y, 1.0f);
    animations->Get(LEVEL_END_ID_ANIMATION_SLOT)->Render(position.x, position.y + yOffset, 1.0f);
}

void CLevelEnd::SetState(int state)
{
    if (this->state == state) return;
    this->state = state;
}

void CLevelEnd::Hit()
{
    LPANIMATION currentReward = CAnimations::GetInstance()->Get(LEVEL_END_ID_ANIMATION_SLOT);
    currentReward->Stop();
    int currentFrame = currentReward->GetCurrentFrameIndex();
    if (currentFrame == 0)
        CGame::GetInstance()->SetNextItemFrame(REWARD_FRAMES_FLOWER);
    else if (currentFrame == 1)
        CGame::GetInstance()->SetNextItemFrame(REWARD_FRAMES_STAR);
    else 
        CGame::GetInstance()->SetNextItemFrame(REWARD_FRAMES_MUSHROOM);
    SetState(LEVEL_END_STATE_EMPTY);
}
