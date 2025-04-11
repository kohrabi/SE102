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

void CFirePiranha::SetState(int state)
{
    switch (state)
    {
    case FIRE_PIRANHA_DOWN:
        {
            timer = FIRE_PIRANHA_DOWN_TIME;
            velocity.y = 0;
        }
        break;
    case FIRE_PIRANHA_MOVE:
        if (position.y <= ogPosition.y)
        {
            timer = FIRE_PIRANHA_MOVE_TIME;
            velocity.y = FIRE_PIRANHA_Y_SPEED;
        }
        else
        {
            timer = FIRE_PIRANHA_MOVE_TIME;
            velocity.y = -FIRE_PIRANHA_Y_SPEED;
        }
        break;
    case FIRE_PIRANHA_UP:
        {
            timer = FIRE_PIRANHA_DOWN_TIME;
            velocity.y = 0;
        }
        break;
    case FIRE_PIRANHA_WAIT:
        {
            timer = FIRE_PIRANHA_UP_TIME;
            velocity.y = 0;
        }
        break;
    }
    this->state = state;
}

void CFirePiranha::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;
    if (timer > 0) timer -= dt;
    else
    {
        if (state == FIRE_PIRANHA_DOWN || state == FIRE_PIRANHA_UP) SetState(FIRE_PIRANHA_MOVE);
        else if (state == FIRE_PIRANHA_MOVE) SetState(FIRE_PIRANHA_WAIT);
        else if (state == FIRE_PIRANHA_WAIT) SetState(FIRE_PIRANHA_MOVE);
    }
    position.y = clampf(position.y + velocity.y * dt, ogPosition.y, ogPosition.y + 32.f);
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
        sprites->Get(animationID)->Draw(position.x, position.y, GetLayer(layer, orderInLayer), flipX);
    }
}