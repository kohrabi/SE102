#include "QuestionBlock.h"

#include "GameObjects/Mario.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "QuestionBlock.h"

#include "Coin.h"
#include "NPC/Mushroom.h"

#include "ContentIds/QuestionBlock.h"

#include "GameObjects/NPC/Goomba.h"
#include "GameObjects/NPC/GreenKoopa.h"
#include "GameObjects/NPC/RedKoopa.h"

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


CQuestionBlock::CQuestionBlock(float x, float y, int type, int count)
    : CGameObject(x, y, 0.0f), spawnType(type), spawnCount(count)
{
    ASSERT(spawnType > 0 && spawnType <= 3, "Invalid spawn type");
    ASSERT(spawnCount >= 0, "Invalid spawn count");
    ogYPos = y;
    LoadContent();
    cast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return (dynamic_cast<CGoomba*>(obj) != NULL) ||
            (dynamic_cast<CGreenKoopa*>(obj) != NULL) ||
            (dynamic_cast<CRedKoopa*>(obj) != NULL);
        });
    layer = SortingLayer::BLOCK;
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

    cast.SetBoundingBox(position - Vector2(0, 16), Vector2(16, 8));
    if (isHit)
    {
        cast.CheckOverlap(coObjects);
        if (cast.collision.size() > 0)
        {
            for (LPGAMEOBJECT obj : cast.collision)
                obj->Delete();
        }
        isHit = false;
    }
}

void CQuestionBlock::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    CSprites* const sprites = CSprites::GetInstance();

    LPANIMATION animation = animations->Get(spawnCount > 0 ? QUESTION_BLOCK_ID_ANIMATION_IDLE : QUESTION_BLOCK_ID_ANIMATION_EMPTY);
    animation->Render(position.x, position.y, GetLayer(layer, orderInLayer));
    cast.RenderBoundingBox();
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
    isHit = true;
    isActive = false;
    spawnCount--;
    animationTimer = QUESTION_BLOCK_ANIMATION_TIME;
}
