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

int CRedKoopa::GetAnimationId()
{
    if (state == KOOPA_STATE_RESPAWNING)
        return RED_KOOPA_ID_ANIMATION_RESPAWN;
    if (state == KOOPA_STATE_NORMAL)
        return RED_KOOPA_ID_ANIMATION_WALK;
    else
    {
        if (state == KOOPA_STATE_DEAD_BOUNCE)
            return RED_KOOPA_ID_ANIMATION_SHELL_IDLE;
        if (abs(velocity.x) > 0)
            return RED_KOOPA_ID_ANIMATION_SHELL;
        return RED_KOOPA_ID_ANIMATION_SHELL_IDLE;
    }
}

CRedKoopa::CRedKoopa(float x, float y) : CGreenKoopa(x, y)
{
    LoadContent();
    nx = -1;
    holdCast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return !(obj == this || !obj->IsBlocking());
    });
    layer = SortingLayer::NPC;
}

void CRedKoopa::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    if (state == KOOPA_STATE_NORMAL)
    {
        holdCast.SetBoundingBox(position + Vector2((7.0f) * nx, 16.0f), Vector2(4.0f, 6.0f));
        holdCast.CheckOverlap(coObjects);
        if (holdCast.collision.size() <= 0)
        {
            nx *= -1;
        }
    }
    CGreenKoopa::Update(dt, coObjects);
}

void CRedKoopa::Render() {
    CGreenKoopa::Render();
    holdCast.RenderBoundingBox();
}