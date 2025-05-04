#include "Leaf.h"

#include "Engine/Loaders/SpritesLoader.h"
#include "GameObjects/Particles/ScorePopup.h"

#include "ContentIds/Leaf.h"

#include <iostream>
#include <algorithm>
#include <Engine/Helper.h>

using namespace std;

bool CLeaf::IsContentLoaded = false;

void CLeaf::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(LEAF_SPRITES_PATH);
}

CLeaf::CLeaf(float x, float y) : CGameObject(x, y, 0.0f)
{
    LoadContent();
    state = LEAF_STATE_INTRO;
    //maxYPos = y - 16;
    velocity.y = LEAF_Y_VELS;
    leafTimer = LEAF_TIMER_VALS;
    nx = 1;
    layer = SortingLayer::CORPSE;
}

void CLeaf::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    
    if (state == LEAF_STATE_INTRO)
    {
        if (leafTimer > 0) leafTimer -= dt;
        if (velocity.y >= 0)
        {
            velocity.y = 0.0f;
            velocity.x = LEAF_X_SPEED * nx;
            leafTimer = LEAF_TIMER_VALS;
            leafStop = false;
            state = LEAF_STATE_NORMAL;
        }
        velocity.y = min(velocity.y + OBJECT_FALL, OBJECT_MAX_FALL);
        position.y += velocity.y * dt;
    }
    else
    {
        if (leafTimer > 0) leafTimer -= dt;
        else
        {
            leafStop = !leafStop;
            leafTimer = LEAF_TIMER_VALS;
            if (leafStop)
            {
                velocity.y = 0.0f;
                //velocity.x = 0.0f;
            }
        }

        if (!leafStop)
        {
            velocity.y = min(velocity.y + OBJECT_FALL, 0x01000 * SUBSUBSUBPIXEL_DELTA_TIME);
        }

        velocity.x = clampf(velocity.x + LEAF_X_SPEED * nx, -LEAF_X_LIMIT, LEAF_X_LIMIT);
        if (velocity.x == -LEAF_X_LIMIT)
            nx = 1;
        if (velocity.x == LEAF_X_LIMIT)
            nx = -1;

        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
        // CCollision::GetInstance()->Process(this, dt, coObjects);
    }
    //cout << velocity.x << '\n';
}

void CLeaf::Render() {
    CSprites* const sprites = CSprites::GetInstance();

    sprites->Get(LEAF_ID_SPRITE_IDLE)->Draw(position.x, position.y, GetLayer(layer, orderInLayer), nx > 0);
}