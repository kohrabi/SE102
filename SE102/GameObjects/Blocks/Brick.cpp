#include "Brick.h"

#include "GameObjects/Mario.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "GameObjects/Particles/Coin.h"
#include "GameObjects/Powerups/Mushroom.h"
#include "GameObjects/Powerups/Leaf.h"

#include "ContentIds/Brick.h"

#include "GameObjects/NPC/Goomba.h"
#include "GameObjects/NPC/Koopa.h"

#include "GameObjects/Powerups/OneUp.h"
#include "ContentIds/Particles.h"

#include <iostream>
using namespace std;

bool CBrick::IsContentLoaded = false;

void CBrick::LoadContent()
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(BRICK_SPRITES_PATH);
}


CBrick::CBrick(float x, float y)
    : CGameObject(x, y, 0.0f)
{
    LoadContent();
    turnCast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return (dynamic_cast<CGoomba*>(obj) != NULL) ||
            (dynamic_cast<CKoopa*>(obj) != NULL);
        });
    layer = SortingLayer::BLOCK;
    SetState(BRICK_STATE_NORMAL);
}

void CBrick::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (state == BRICK_STATE_NORMAL)
    {
        turnCast.SetBoundingBox(position - Vector2(0, 16), Vector2(16, 8));
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
            SetState(BRICK_STATE_BREAK);
            isHit = false;
        }
        //CCollision::GetInstance()->Process(this, dt, coObjects);
    }
    else if (state == BRICK_STATE_BREAK)
    {
        CGame* const game = CGame::GetInstance();
        float cx, cy;
        game->GetCamPos(cx, cy);
        int shouldDestroy = 0;
        for (int i = 0; i < 4; i++)
        {
            debrisVel[i].y = debrisVel[i].y + OBJECT_FALL * 2.0f;

            debrisPos[i].x += debrisVel[i].x * dt;
            debrisPos[i].y += debrisVel[i].y * dt;
            if (debrisPos[i].y > cy + game->GetBackBufferHeight() + 20.0f)
                shouldDestroy++;
        }
        if (shouldDestroy == 4)
        {
            Delete();
        }
    }
}

void CBrick::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    if (state == BRICK_STATE_NORMAL)
    {
        animations->Get(BRICK_ID_ANIMATION_IDLE)->Render(position.x, position.y, GetLayer(layer, orderInLayer));
        turnCast.RenderBoundingBox();
    }
    else if (state == BRICK_STATE_BREAK)
    {
        for (int i = 0; i < 4; i++)
        {
            animations->Get(PARTICLES_ID_ANIMATION_BRICK_DEBRIS)->Render(debrisPos[i].x, debrisPos[i].y, 1.0f);
        }
    }
    RenderBoundingBox();
}

void CBrick::SetState(int state)
{
    if (this->state == state) return;
    if (state == BRICK_STATE_BREAK)
    {
        layer = SortingLayer::CORPSE;
        constexpr float offset = 8.0f;
        debrisPos[0] = position + Vector2(-1.0f, -1.0f) * offset;
        debrisPos[1] = position + Vector2(1.0f, -1.0f) * offset;
        debrisPos[2] = position + Vector2(-1.0f, 1.0f) * offset;
        debrisPos[3] = position + Vector2(1.0f, 1.0f) * offset;

        constexpr float xVel = 0x01400 * SUBSUBSUBPIXEL_DELTA_TIME;
        constexpr float yVel = 0x06000 * SUBSUBSUBPIXEL_DELTA_TIME;
        debrisVel[0] = Vector2(-xVel, -yVel);
        debrisVel[1] = Vector2(xVel, -yVel);
        debrisVel[2] = Vector2(-xVel, -yVel / 2.0f);
        debrisVel[3] = Vector2(xVel, -yVel / 2.0f);
    }
    this->state = state;
}

void CBrick::Hit()
{
    SetState(BRICK_STATE_BREAK);
    isHit = true;
}

void CBrick::OnDelete()
{
    if (coin != nullptr && !coin->IsDeleted()) {
        dynamic_cast<CCoin*>(coin)->SetNoScore();
        coin->Delete();
    }
}

void CBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CKoopa*>(e->obj) && e->dy == 0.0f)
    {
        dynamic_cast<CKoopa*>(e->obj)->TurnAround();
        Hit();
    }
}

void CBrick::SwitchToBrick()
{
    if (coin == nullptr || coin->GetState() == 0 || coin->IsDeleted()) {
        Delete();
        return;
    }
    CCoin* tempCoin = dynamic_cast<CCoin*>(coin);
    if (tempCoin == nullptr)
        return;
    tempCoin->SetNoScore();
    tempCoin->Delete();
    coin = NULL;
    SetState(BRICK_STATE_NORMAL);
}

void CBrick::SwitchToCoin()
{
    if (state == BRICK_STATE_BREAK)
        return;
    SetState(BRICK_STATE_COIN);
    coin = new CCoin(position.x, position.y);
    CGame::GetInstance()->GetCurrentScene()->AddObject(coin);
}
