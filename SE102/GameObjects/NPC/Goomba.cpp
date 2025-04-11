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

void CGoomba::OnNoCollision(DWORD dt)
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

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;
    if (kill)
    {
        killTimer -= dt;
        if (killTimer < 0)
            Delete();
        return;
    }
    velocity.x = GOOMBA_X_SPEED * nx;
    velocity.y += OBJECT_FALL;
    velocity.y = min(velocity.y, OBJECT_MAX_FALL);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGoomba::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    if (kill)
        animations->Get(GOOMBA_ID_ANIMATION_DEAD)->Render(position.x, position.y, GetLayer(layer, orderInLayer));
    else
        animations->Get(GOOMBA_ID_ANIMATION_WALK)->Render(position.x, position.y, GetLayer(layer, orderInLayer));
}