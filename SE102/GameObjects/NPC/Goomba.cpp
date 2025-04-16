#include "Goomba.h"

#include "Engine/Loaders/SpritesLoader.h"
#include "GameObjects/ScorePopup.h"

#include "ContentIds/Goomba.h"
#include "ContentIds/Wing.h"
#include <iostream>
#include <algorithm>
#include <Engine/Helper.h>

using namespace std;

bool CGoomba::IsContentLoaded = false;

void CGoomba::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(GOOMBA_SPRITES_PATH);
    loader.Load(WING_SPRITES_PATH);
}

void CGoomba::SetState(int state)
{
    switch (state)
    {
    case GOOMBA_STATE_DEAD:
    {
        if (hasWing)
        {
            hasWing = false;
            this->state = GOOMBA_STATE_NORMAL;
            return;
        }
        else
        {
            CGame* const game = CGame::GetInstance();
            game->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y));
            killTimer = GOOMBA_KILL_TIME;
        }
    }
    break;
    case GOOMBA_STATE_DEAD_BOUNCE:
    {
        CGame* const game = CGame::GetInstance();
        game->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y));
        layer = SortingLayer::CORPSE;
        velocity.y = -OBJECT_DEAD_BOUNCE;
        velocity.x = OBJECT_DEAD_X_VEL;
    }
    break;
    default: break;
    }
    this->state = state;
}

void CGoomba::OnNoCollision(float dt)
{
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{

    if (e->nx != 0 && e->obj->IsBlocking())
    {
        nx *= -1;
    }
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        //if (hasWing)
        //    velocity.y += 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;
        //else
            velocity.y = 0.0f;
    }
    
}

void CGoomba::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;
    if (hasWing)
    {
        velocity.x = GOOMBA_X_SPEED * nx;
        //velocity.x = clampf(velocity.x + GOOMBA_WING_X_SPEED * nx, -GOOMBA_WING_X_MAX_SPEED, GOOMBA_WING_X_MAX_SPEED);
        velocity.y = min(velocity.y + OBJECT_FALL / 1.5f, OBJECT_MAX_FALL / 1.5);
        if (wingActivateTimer > 0) wingActivateTimer -= dt;
        else
        {
            wingActivate = !wingActivate;
            wingActivateTimer = GOOMBA_WING_ACTIVATE;
            hopCount = 0;
            hopTimer = GOOMBA_WING_HOP_TIME;
            if (!wingActivate)
            {
                LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
                if (player != NULL)
                {
                    nx = sign(player->GetPosition().x - position.x);
                }
            }
        }
        if (wingActivate && hopCount <= GOOMBA_WING_HOP_COUNT)
        {
            if (hopTimer > 0) hopTimer -= dt;
            else
            {
                if (hopCount == 3)
                    velocity.y = -GOOMBA_WING_BIG_HOP;
                else 
                    velocity.y = -GOOMBA_WING_HOP;
                hopCount = (hopCount + 1);
                hopTimer = GOOMBA_WING_HOP_TIME;
            }
        }
        CCollision::GetInstance()->Process(this, dt, coObjects);
        return;
    }
    switch (state)
    {
    case GOOMBA_STATE_NORMAL:
        {
            velocity.y += OBJECT_FALL;
            velocity.y = min(velocity.y, OBJECT_MAX_FALL);
            velocity.x = GOOMBA_X_SPEED * nx;
            CCollision::GetInstance()->Process(this, dt, coObjects);
        }
        break;
    case GOOMBA_STATE_DEAD:
        {
            killTimer -= dt;
            if (killTimer < 0)
                Delete();
        }
        break;
    case GOOMBA_STATE_DEAD_BOUNCE:
        {
            velocity.y += OBJECT_FALL;
            velocity.y = min(velocity.y, OBJECT_MAX_FALL);
            velocity.x = GOOMBA_X_SPEED * nx;

            // Handle physics here
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;
        }
        break;
    }
}

void CGoomba::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    if (hasWing)
    {
        if (wingActivate)
        {
            animations->Get(WING_ID_ANIMATION_WING_ACTIVE)->Render(position.x - 6.0f, position.y - 5.0f, GetLayer(layer, orderInLayer), true);
            animations->Get(WING_ID_ANIMATION_WING_ACTIVE)->Render(position.x + 6.0f, position.y - 5.0f, GetLayer(layer, orderInLayer));
        }
        else
        {
            animations->Get(WING_ID_ANIMATION_WING_IDLE)->Render(position.x - 6.0f, position.y - 5.0f, GetLayer(layer, orderInLayer), true);
            animations->Get(WING_ID_ANIMATION_WING_IDLE)->Render(position.x + 6.0f, position.y - 5.0f, GetLayer(layer, orderInLayer));

        }
    }
    if (state == GOOMBA_STATE_DEAD)
        animations->Get(GOOMBA_ID_ANIMATION_DEAD)->Render(position.x, position.y, GetLayer(layer, orderInLayer), false);
    else
        animations->Get(GOOMBA_ID_ANIMATION_WALK)->Render(position.x, position.y, GetLayer(layer, orderInLayer), false, state == GOOMBA_STATE_DEAD_BOUNCE);
}