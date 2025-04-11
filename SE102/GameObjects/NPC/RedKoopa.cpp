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

CRedKoopa::CRedKoopa(float x, float y) : CGreenKoopa(x, y)
{
    LoadContent();
    nx = -1;
    cast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return !(obj == this || obj->GetVelocity().length() > 0);
    });
    layer = SortingLayer::NPC;
}

void CRedKoopa::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    if (!inShell)
    {
        cast.SetBoundingBox(position + Vector2((7) * nx, 16), Vector2(4, 6));
        cast.CheckOverlap(coObjects);
        if (cast.collision.size() <= 0)
        {
            nx *= -1;
        }
    }
    CGreenKoopa::Update(dt, coObjects);
}

void CRedKoopa::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    if (!inShell)
    {
        animations->Get(RED_KOOPA_ID_ANIMATION_WALK)->Render(position.x, position.y - 0, GetLayer(layer, orderInLayer), nx > 0);
    }
    else
    {
        auto animation = animations->Get(RED_KOOPA_ID_ANIMATION_SHELL);
        if (abs(velocity.x) > 0)
            animation->Play();
        else
            animation->Stop();
        animation->Render(position.x, position.y + 8, GetLayer(layer, orderInLayer), nx > 0, true);

    }
    cast.Render();
}