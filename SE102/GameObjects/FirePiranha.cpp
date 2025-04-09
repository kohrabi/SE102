#include "FirePiranha.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/FirePiranha.h"

#include "Engine/Helper.h"

#include <iostream>
#include <algorithm>

using namespace std;

bool CFirePiranha::IsContentLoaded = false;

void CFirePiranha::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(FIRE_PIRANHA_SPRITES_PATH);
}

void CFirePiranha::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (timer > 0) timer -= dt;
    else
    {
        switch (state)
        {
        case FIRE_PIRANHA_DOWN:
        {
            state = FIRE_PIRANHA_MOVE;
            timer = FIRE_PIRANHA_MOVE_TIME;
            velocity.y = FIRE_PIRANHA_Y_SPEED;
        }
        break;
        case FIRE_PIRANHA_MOVE:
        break;
        case FIRE_PIRANHA_UP:
        {
            state = FIRE_PIRANHA_MOVE;
            timer = FIRE_PIRANHA_MOVE_TIME;
            velocity.y = FIRE_PIRANHA_Y_SPEED;
        }
        break;
        case FIRE_PIRANHA_WAIT:
        {
            state = FIRE_PIRANHA_MOVE;
        }
        break;
        }
    }
    position.y += velocity.y * dt;
}

void CFirePiranha::Render() {
    CSprites* const sprites = CSprites::GetInstance();
    CGame* const game = CGame::GetInstance();
    LPGAMEOBJECT object = game->GetCurrentScene()->GetPlayer();
    
    if (object != NULL)
    {
        Vector2 playerPosition = object->GetPosition() - position;
        bool flipX = false;
        int animationID = FIRE_PIRANHA_ID_SPRITE_LOOK_DOWN_CLOSE;
        if (sign(playerPosition.x) > 0)
            flipX = true;
        if (sign(playerPosition.y) < 0)
            animationID = FIRE_PIRANHA_ID_SPRITE_LOOK_UP_CLOSE;
        sprites->Get(animationID)->Draw(position.x, position.y, flipX);
    }
}