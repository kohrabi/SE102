#include "QuestionBlock.h"

#include "GameObjects/Mario.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "contents.h"

#include "GameObjects/Coin.h"

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
    if (!isActive)
        return;
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBlock::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    CSprites* const sprites = CSprites::GetInstance();

    LPANIMATION animation = animations->Get(isActive ? QUESTION_BLOCK_ID_ANIMATION_NORMAL : QUESTION_BLOCK_ID_ANIMATION_EMPTY);
    animation->Render(position.x, position.y);
    // RenderBoundingBox();
}

void CQuestionBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CMario*>(e->obj) && e->ny < 0) 
    {
        CMario* const mario = dynamic_cast<CMario*>(e->obj);
        CGame* const game = CGame::GetInstance();
        game->GetCurrentScene()->AddObject(new CCoin(position.x, position.y));
        isActive = false;
    }
}
