#include "Goomba.h"

#include "Engine/Loaders/SpritesLoader.h"
#include "GameObjects/Particles/ScorePopup.h"

#include "ContentIds/Goomba.h"
#include "ContentIds/Wing.h"
#include <iostream>
#include <algorithm>
#include <Engine/Helper.h>
#include "ContentIds/RedGoomba.h"

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
    loader.Load(RED_GOOMBA_SPRITES_PATH);
}

void CGoomba::SetState(int state)
{
    if (this->state == state)
        return;
    switch (state)
    {
    case GOOMBA_STATE_DEAD:
    {
        if (ignoreDamageTimer > 0) return;
        CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y));
        if (this->state == GOOMBA_STATE_WING)
        {
            state = GOOMBA_STATE_NORMAL;
            velocity.x = 0.0f;
            ignoreDamageTimer = GOOMA_IGNORE_DAMAGE_TIME;
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
        if (ignoreDamageTimer > 0) return;
        if (this->state == GOOMBA_STATE_DEAD || isDeleted)
            return;
        CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y));
        if (this->state == GOOMBA_STATE_WING)
        {
            state = GOOMBA_STATE_NORMAL;
            velocity.x = 0.0f;
            ignoreDamageTimer = GOOMA_IGNORE_DAMAGE_TIME;
        }
        else
        {
            CGame* const game = CGame::GetInstance();
            game->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y));
            layer = SortingLayer::CORPSE;
            velocity.y = -OBJECT_DEAD_BOUNCE;
            velocity.x = OBJECT_DEAD_X_VEL;
        }
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
    onGround = false;
}

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{

    if (e->nx != 0 && e->obj->IsBlocking())
    {
        nx *= -1;
    }
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        if (state == GOOMBA_STATE_WING && e->ny > 0)
            velocity.y = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;
        else
            velocity.y = 0.0f;
        if (e->ny < 0)
            onGround = true;
    }
    
}

void CGoomba::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;
    if (ignoreDamageTimer > 0) ignoreDamageTimer -= dt;
   
    switch (state)
    {
    case GOOMBA_STATE_WING:
    {
        velocity.x = GOOMBA_X_SPEED * nx;
        velocity.y = min(velocity.y + OBJECT_FALL / 1.5f, OBJECT_MAX_FALL / 1.5f);

        if (hopTimer > 0) hopTimer -= dt;
        if (wingActivateTimer > 0) wingActivateTimer -= dt;
        if (changeDirTimer > 0) changeDirTimer -= dt;
        
        if (onGround)
        {
            if (wingActivateTimer <= 0)
            {
                if (hopTimer <= 0)
                {
                    hopTimer = GOOMBA_WING_HOP_TIME;
                    hopCount = (hopCount + 1);
                    velocity.y = -GOOMBA_WING_HOP;
                    if (hopCount == 4)
                    {
                        velocity.y = -GOOMBA_WING_BIG_HOP;
                        wingActivateTimer = GOOMBA_WING_ACTIVATE_TIME;
                        hopCount = 0;
                    }
                }
            }
            // Change Goomba direction to player
            else if (changeDirTimer <= 0)
            {
                LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
                if (player != NULL)
                {
                    nx = sign(player->GetPosition().x - position.x);
                }
            }
        }
        CCollision::GetInstance()->Process(this, dt, coObjects);

    }
    break;
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
        if (!IsColliderInCamera())
            Delete();
    }
    break;
    }
}

void CGoomba::Render() {
    CAnimations* const animations = CAnimations::GetInstance();


    if (state == GOOMBA_STATE_WING)
    {
        const Vector2 wingOffset = Vector2(6.0f, 10.0f);
        if (wingActivateTimer <= 0.0f || (wingActivateTimer >= 0.0f && !onGround))
        {
            animations->Get(WING_ID_ANIMATION_WING_ACTIVE)->Render(position.x - wingOffset.x, position.y - wingOffset.y, GetLayer(layer, orderInLayer), true);
            animations->Get(WING_ID_ANIMATION_WING_ACTIVE)->Render(position.x + wingOffset.x, position.y - wingOffset.y, GetLayer(layer, orderInLayer));
        }
        else
        {
            animations->Get(WING_ID_ANIMATION_WING_IDLE)->Render(position.x - wingOffset.x, position.y - wingOffset.y, GetLayer(layer, orderInLayer), true);
            animations->Get(WING_ID_ANIMATION_WING_IDLE)->Render(position.x + wingOffset.x, position.y - wingOffset.y, GetLayer(layer, orderInLayer));

        }
    }

    if (isRedGoomba)
    {
        if (state == GOOMBA_STATE_DEAD)
            animations->Get(RED_GOOMBA_ID_ANIMATION_DEAD)->Render(position.x, position.y, GetLayer(layer, orderInLayer), false);
        else
            animations->Get(RED_GOOMBA_ID_ANIMATION_WALK)->Render(position.x, position.y, GetLayer(layer, orderInLayer), false, state == GOOMBA_STATE_DEAD_BOUNCE);
    }
    else
    {
        if (state == GOOMBA_STATE_DEAD)
            animations->Get(GOOMBA_ID_ANIMATION_DEAD)->Render(position.x, position.y, GetLayer(layer, orderInLayer), false);
        else
            animations->Get(GOOMBA_ID_ANIMATION_WALK)->Render(position.x, position.y, GetLayer(layer, orderInLayer), false, state == GOOMBA_STATE_DEAD_BOUNCE);
    }
}