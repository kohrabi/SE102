#include "Brick.h"

#include "GameObjects/Mario.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "GameObjects/Particles/Coin.h"
#include "GameObjects/Powerups/Mushroom.h"
#include "GameObjects/Powerups/Leaf.h"

#include "ContentIds/Brick.h"

#include "GameObjects/NPC/Goomba.h"
#include "GameObjects/NPC/GreenKoopa.h"
#include "GameObjects/NPC/RedKoopa.h"

#include "GameObjects/Powerups/OneUp.h"

#include <iostream>
using namespace std;

bool CBrick::IsContentLoaded = false;

void CBrick::LoadContent()
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(BRICK_SPRITES_PATH);
}


CBrick::CBrick(float x, float y)
    : CGameObject(x, y, 0.0f)
{
    LoadContent();
    holdCast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return (dynamic_cast<CGoomba*>(obj) != NULL) ||
            (dynamic_cast<CGreenKoopa*>(obj) != NULL) ||
            (dynamic_cast<CRedKoopa*>(obj) != NULL);
        });
    layer = SortingLayer::BLOCK;
}

void CBrick::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    holdCast.SetBoundingBox(position - Vector2(0, 16), Vector2(16, 8));
    if (isHit)
    {
        holdCast.CheckOverlap(coObjects);
        if (holdCast.collision.size() > 0)
        {
            for (LPGAMEOBJECT obj : holdCast.collision)
            {
                if (dynamic_cast<CGoomba*>(obj))
                {
                    CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
                    goomba->DeadBounce();
                }
                else if (dynamic_cast<CGreenKoopa*>(obj))
                {
                    CGreenKoopa* greenKoopa = dynamic_cast<CGreenKoopa*>(obj);
                    greenKoopa->DeadBounce();
                }
                else if (dynamic_cast<CRedKoopa*>(obj))
                {
                    CRedKoopa* redKoopa = dynamic_cast<CRedKoopa*>(obj);
                    redKoopa->DeadBounce();
                }
            }
        }
        Delete();
        isHit = false;
    }
}

void CBrick::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    animations->Get(BRICK_ID_ANIMATION_IDLE)->Render(position.x, position.y, GetLayer(layer, orderInLayer));
    holdCast.RenderBoundingBox();
}

void CBrick::Hit(int dx)
{
    if (dx == 0)
        Delete();
    else
        isHit = true;
}
