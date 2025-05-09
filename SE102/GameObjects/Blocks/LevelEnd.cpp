#include "LevelEnd.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/LevelEnd.h"
#include "ContentIds/HUD.h"

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
    loader.Load(HUD_SPRITES_PATH);
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
    if (timer > 0) timer -= dt;
    else if (course <= 3)
    {
        timer = LEVEL_END_COURSE_TIME;
        course++;
    }
}

void CLevelEnd::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    CSprites* const sprites = CSprites::GetInstance();


    sprites->Get(LEVEL_END_ID_SPRITE_BOX)->Draw(position.x, position.y, 1.0f);
    if (currentRewardSlot == 0)
    {
        animations->Get(LEVEL_END_ID_ANIMATION_SLOT)->Render(position.x, position.y, 1.0f);
    }
    else
    {
        if (course >= 2)
        {
            sprites->Get(LEVEL_END_ID_SPRITE_COURSE_CLEAR)->Draw(position.x, position.y - 72.0f, 1.0f);
            if (course >= 3)
                sprites->Get(LEVEL_END_ID_SPRITE_YOU_GOT_A_CARD)->Draw(position.x - 20.0f, position.y - 56.0f, 1.0f);
        }
        if (currentRewardSlot == REWARD_FRAMES_MUSHROOM)
        {
            animations->Get(LEVEL_END_ID_ANIMATION_MUSHROOM)->Render(position.x, position.y + yOffset, 1.0f);
            if (course >= 3)
                sprites->Get(HUD_ID_SPRITE_REWARDS_FRAME_0)->Draw(position.x + 60.0f, position.y - 46.0f, 1.0f);
        }
        else if (currentRewardSlot == REWARD_FRAMES_FLOWER)
        {
            animations->Get(LEVEL_END_ID_ANIMATION_FLOWER)->Render(position.x, position.y + yOffset, 1.0f);
            if (course >= 3)
                sprites->Get(HUD_ID_SPRITE_REWARDS_FRAME_1)->Draw(position.x + 60.0f, position.y - 46.0f, 1.0f);
        }
        else
        {
            animations->Get(LEVEL_END_ID_ANIMATION_STAR)->Render(position.x, position.y + yOffset, 1.0f);
            if (course >= 3)
                sprites->Get(HUD_ID_SPRITE_REWARDS_FRAME_2)->Draw(position.x + 60.0f, position.y - 46.0f, 1.0f);

        }
    }

}

void CLevelEnd::SetState(int state)
{
    if (this->state == state) return;
    this->state = state;
}

void CLevelEnd::Hit()
{
    LPANIMATION currentRewardAnimation = CAnimations::GetInstance()->Get(LEVEL_END_ID_ANIMATION_SLOT);
    currentRewardAnimation->Stop();
    int currentFrame = currentRewardAnimation->GetCurrentFrameIndex();
    timer = LEVEL_END_COURSE_TIME * 2;
    course = 1;
    if (currentFrame == 0)
    {
        currentRewardSlot = REWARD_FRAMES_MUSHROOM;
        CGame::GetInstance()->SetNextItemFrame(REWARD_FRAMES_MUSHROOM);
    }
    else if (currentFrame == 1)
    {
        currentRewardSlot = REWARD_FRAMES_FLOWER;
        CGame::GetInstance()->SetNextItemFrame(REWARD_FRAMES_FLOWER);
    }
    else
    {
        currentRewardSlot = REWARD_FRAMES_STAR;
        CGame::GetInstance()->SetNextItemFrame(REWARD_FRAMES_STAR);
    }
    SetState(LEVEL_END_STATE_EMPTY);
}
