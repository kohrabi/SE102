#include "QuestionBlock.h"

#include "GameObjects/Mario.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "GameObjects/Particles/Coin.h"
#include "GameObjects/Powerups/Mushroom.h"
#include "GameObjects/Powerups/Leaf.h"

#include "ContentIds/QuestionBlock.h"

#include "GameObjects/NPC/Goomba.h"
#include "GameObjects/NPC/Koopa.h"

#include "GameObjects/Powerups/OneUp.h"

#include <iostream>
#include <ContentIds/Brick.h>
#include "PButton.h"
using namespace std;

bool CQuestionBlock::IsContentLoaded = false;

void CQuestionBlock::LoadContent()
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(QUESTION_BLOCK_SPRITES_PATH);
    loader.Load(BRICK_SPRITES_PATH);
}


CQuestionBlock::CQuestionBlock(float x, float y, int type, int count)
    : CGameObject(x, y, 0.0f), spawnType(type), spawnCount(count)
{
    ASSERT(spawnType > 0 && spawnType <= 4, "Invalid spawn type");
    ASSERT(spawnCount >= 0, "Invalid spawn count");

    turnCast.SetBoundingBox(position - Vector2(0, 16), Vector2(16, 8));
    LoadContent();
    turnCast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return (dynamic_cast<CGoomba*>(obj) != NULL) ||
            (dynamic_cast<CKoopa*>(obj) != NULL);
        });
    layer = SortingLayer::BLOCK;
}

void CQuestionBlock::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (animationTimer >= 0) 
    {
        animationTimer -= dt;
        if (animationTimer >= QUESTION_BLOCK_ANIMATION_TIME / 2)
            yOffset -= QUESTION_BLOCK_ANIMATION_Y_VEL * dt;
        else
            yOffset += QUESTION_BLOCK_ANIMATION_Y_VEL * dt;
        yOffset = min(yOffset, 0);
    }
    //position.y = min(position.y, ogYPos);
    if (isActive) {
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }

    if (isHit)
    {
        turnCast.CheckOverlap(coObjects);
        if (turnCast.collision.size() > 0)
        {
            for (LPGAMEOBJECT obj : turnCast.collision)
            {
                if (dynamic_cast<CGoomba*>(obj))
                {
                    CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
                    goomba->DeadBounce();
                }
                else if (dynamic_cast<CKoopa*>(obj))
                {
                    CKoopa* greenKoopa = dynamic_cast<CKoopa*>(obj);
                    greenKoopa->DeadBounce();
                }
            }
        }
        isHit = false;
    }
}

void CQuestionBlock::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    CSprites* const sprites = CSprites::GetInstance();

    if (spawnCount > 0)
    {
        LPANIMATION animation = animations->Get(isBrick ? BRICK_ID_ANIMATION_IDLE : QUESTION_BLOCK_ID_ANIMATION_IDLE);
        animation->Render(position.x, position.y + yOffset, GetLayer(layer, orderInLayer));
    }
    else
    {
        animations->Get(QUESTION_BLOCK_ID_ANIMATION_EMPTY)->Render(position.x, position.y + yOffset, GetLayer(layer, orderInLayer));

    }
    turnCast.RenderBoundingBox();
}

void CQuestionBlock::Hit(int dx)
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
    case QUESTION_BLOCK_SPAWN_LEAF:
    {
        CMario* player = dynamic_cast<CMario*>(game->GetCurrentScene()->GetPlayer());
        if (player != NULL)
        {
            LPGAMEOBJECT powerUp = NULL;
            if (player->GetPowerUp() == MARIO_POWERUP_SMALL)
            {
                powerUp = new CMushroom(position.x, position.y);
                powerUp->SetNx(dx);
            }
            else
            {
                powerUp = new CLeaf(position.x, position.y);
                powerUp->SetNx(dx);
            }
            game->GetCurrentScene()->AddObject(powerUp); 
        }
    }
    break;
    case QUESTION_BLOCK_SPAWN_ONE_UP:
    {
        CMario* player = dynamic_cast<CMario*>(game->GetCurrentScene()->GetPlayer());
        LPGAMEOBJECT powerUp = NULL;
        powerUp = new COneUp(position.x, position.y);
        powerUp->SetNx(dx);
        game->GetCurrentScene()->AddObject(powerUp);

    }
    break;
    case QUESTION_BLOCK_SPAWN_P_BUTTON:
    {
        CPButton* button = new CPButton(position.x, position.y - 16.0f);
        game->GetCurrentScene()->AddObject(button);

    }
    break;
    }
    isHit = true;
    isActive = false;
    spawnCount--;
    animationTimer = QUESTION_BLOCK_ANIMATION_TIME;
}
