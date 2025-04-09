#include "QuestionBlock.h"

#include "GameObjects/Mario.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "QuestionBlock.h"

#include "GameObjects/Coin.h"
#include "GameObjects/Mushroom.h"

#include "ContentIds/QuestionBlock.h"

#include <iostream>
using namespace std;

bool CQuestionBlock::IsContentLoaded = false;

void CQuestionBlock::LoadContent()
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(QUESTION_BLOCK_SPRITES_PATH);
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (animationTimer >= 0) 
    {
        animationTimer -= dt;
        if (animationTimer >= QUESTION_BLOCK_ANIMATION_TIME / 2)
            position.y -= QUESTION_BLOCK_ANIMATION_Y_VEL * dt;
        else
            position.y += QUESTION_BLOCK_ANIMATION_Y_VEL * dt;
    }
    position.y = min(position.y, ogYPos);
    if (isActive) {
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
}

void CQuestionBlock::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    CSprites* const sprites = CSprites::GetInstance();

    LPANIMATION animation = animations->Get(spawnCount > 0 ? QUESTION_BLOCK_ID_ANIMATION_IDLE : QUESTION_BLOCK_ID_ANIMATION_EMPTY);
    animation->Render(position.x, position.y);
    // RenderBoundingBox();
}

void CQuestionBlock::Hit()
{
    if (spawnCount <= 0)
        return;
    CGame* const game = CGame::GetInstance();
    switch (spawnType)
    {
    case QUESTION_BLOCK_SPAWN_COIN:
    {
        CCoin* coin = new CCoin(position.x, position.y);
        coin->SetState(COIN_STATE_INTRO);
        game->GetCurrentScene()->AddObject(coin); 
    }
    break;
    case QUESTION_BLOCK_SPAWN_MUSHROOM:
    {
        game->GetCurrentScene()->AddObject(new CMushroom(position.x, position.y)); 
    }
    break;
    default:
    break;
    }
    isActive = false;
    spawnCount--;
    animationTimer = QUESTION_BLOCK_ANIMATION_TIME;
}
