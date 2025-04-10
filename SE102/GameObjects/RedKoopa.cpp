#include "RedKoopa.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/RedKoopa.h"
#include <iostream>
#include <algorithm>
#include <Engine/Helper.h>

using namespace std;

bool CRedKoopa::IsContentLoaded = false;

void CRedKoopa::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(RED_KOOPA_SPRITES_PATH);
}

void CRedKoopa::PlayerHit(int nx)
{
    if (!inShell)
    {
        velocity.x = 0;
        inShell = true;
        this->nx = 0;
    }
    else
    {
        if (this->nx == -nx)
            this->nx = 0;
        else
            this->nx = -nx;
    }
}

void CRedKoopa::OnNoCollision(DWORD dt)
{
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void CRedKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->nx != 0 && e->obj->IsBlocking())
    {
        nx *= -1;
    }
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        velocity.y = 0.0f;
    }
    
}

void CRedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    cast.SetBoundingBox(position + Vector2((7) * nx, 16), Vector2(4, 6));
    cast.CheckOverlap(coObjects);
    if (cast.collisionCount <= 0)
    {
        nx *= -1;
    }

    if (!inShell)
    {
        velocity.x = RED_KOOPA_X_SPEED * nx;
        velocity.y += OBJECT_FALL;
        velocity.y = min(velocity.y, OBJECT_MAX_FALL);
    }
    else
    {
        velocity.x = RED_KOOPA_SHELL_X_SPEED * nx;
    }
    CCollision::GetInstance()->Process(this, dt, coObjects);


}

void CRedKoopa::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    if (!inShell)
    {
        animations->Get(RED_KOOPA_ID_ANIMATION_WALK)->Render(position.x, position.y - 0, nx > 0 ? true : false);
    }
    else
    {
        auto animation = animations->Get(RED_KOOPA_ID_ANIMATION_SHELL);
        if (abs(velocity.x) > 0)
            animation->Play();
        else
            animation->Stop();
        animation->Render(position.x, position.y + 8, nx > 0 ? true : false);

    }
    cast.Render();
    RenderBoundingBox();
}