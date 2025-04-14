#include "Goomba.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/Goomba.h"
#include <iostream>
#include <algorithm>

using namespace std;

bool CGoomba::IsContentLoaded = false;

void CGoomba::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(GOOMBA_SPRITES_PATH);
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
        velocity.x = 0.0f;
    }
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        velocity.y = 0.0f;
    }
    
}

void CGoomba::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;
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

    if (state == GOOMBA_STATE_DEAD)
        animations->Get(GOOMBA_ID_ANIMATION_DEAD)->Render(position.x, position.y, GetLayer(layer, orderInLayer), false);
    else
        animations->Get(GOOMBA_ID_ANIMATION_WALK)->Render(position.x, position.y, GetLayer(layer, orderInLayer), false, state == GOOMBA_STATE_DEAD_BOUNCE);
}