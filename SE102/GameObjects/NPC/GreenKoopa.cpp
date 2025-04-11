#include "GreenKoopa.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/GreenKoopa.h"
#include <iostream>
#include <algorithm>
#include <Engine/Helper.h>

using namespace std;

bool CGreenKoopa::IsContentLoaded = false;

void CGreenKoopa::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(GREEN_KOOPA_SPRITES_PATH);
}

void CGreenKoopa::PlayerHit(int nx)
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

void CGreenKoopa::AttachHold(LPGAMEOBJECT player, float holdYOffset)
{
    this->player = player;
    this->holdYOffset = holdYOffset;
}

void CGreenKoopa::DetachHold()
{
    this->player = NULL;
    position.y += holdYOffset / 2.0f;
    holdYOffset = 0;

}

void CGreenKoopa::OnNoCollision(DWORD dt)
{
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void CGreenKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
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

void CGreenKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    velocity.y += OBJECT_FALL;
    velocity.y = min(velocity.y, OBJECT_MAX_FALL);

    if (!inShell)
    {
        velocity.x = GREEN_KOOPA_X_SPEED * nx;   
    }
    else
    {
        if (player != NULL)
        {
            nx = player->GetNx();
            position = player->GetPosition() + Vector2(16 * nx, holdYOffset);
            velocity.x = 0;
            velocity.y = 0;
        }
        else
        {
            velocity.x = GREEN_KOOPA_SHELL_X_SPEED * nx;
        }
    }
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGreenKoopa::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    if (!inShell)
    {
        animations->Get(GREEN_KOOPA_ID_ANIMATION_WALK)->Render(position.x, position.y - 0);
    }
    else
    {
        auto animation = animations->Get(GREEN_KOOPA_ID_ANIMATION_SHELL);
        if (abs(velocity.x) > 0)
            animation->Play();
        else
            animation->Stop();
        animation->Render(position.x, position.y + 8, nx > 0, true);

    }
    RenderBoundingBox();
}