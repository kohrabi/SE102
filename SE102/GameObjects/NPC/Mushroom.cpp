#include "Mushroom.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/Mushroom.h"

#include <iostream>
#include <algorithm>

using namespace std;

bool CMushroom::IsContentLoaded = false;

void CMushroom::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(MUSHROOM_SPRITES_PATH);
}

void CMushroom::OnNoCollision(DWORD dt)
{
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->nx != 0 && e->obj->IsBlocking())
    {
        nx = (int)e->nx;
    }
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        velocity.y = 0.0f;
    }
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (state == MUSHROOM_STATE_INTRO)
    {
        position.y -= MUSHROOM_INTRO_Y_VELOCITY * dt;
        position.y = max(position.y, maxYPos);
        if (abs(position.y - maxYPos) <= 0.01f)
        {
            inIntro = false;
            state = MUSHROOM_STATE_NORMAL;
        }
    }
    else
    {
        velocity.x = MUSHROOM_X_SPEED * nx;
        velocity.y += OBJECT_FALL;
        velocity.y = min(velocity.y, OBJECT_MAX_FALL);
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
}

void CMushroom::Render() {
    CSprites* const sprites = CSprites::GetInstance();

    sprites->Get(MUSHROOM_ID_SPRITE_IDLE)->Draw(position.x, position.y);
}