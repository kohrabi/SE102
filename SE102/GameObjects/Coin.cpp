#include "Coin.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/Coin.h"

#include <iostream>
#include <algorithm>

using namespace std;

bool CCoin::IsContentLoaded = false;

void CCoin::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(COIN_SPRITES_PATH);
}


void CCoin::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case COIN_STATE_INTRO:
        {
            startTime = (ULONGLONG)GetTickCount64() + COIN_DESTROY_TIME;
            maxYPos = position.y + COIN_MAX_Y;
        }
        break;
    }
}

void CCoin::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state != COIN_STATE_INTRO)
        return;
    position.y -= COIN_Y_VELOCITY * dt;

    position.y = max(position.y, maxYPos);
    ULONGLONG now = GetTickCount64();
    if (now > startTime) {
        Delete();
        cout << "delete\n";
    }
}

void CCoin::Render() {
    CAnimations* const animations = CAnimations::GetInstance();
    CSprites* const sprites = CSprites::GetInstance();

    if (state == COIN_STATE_INTRO)
        animations->Get(COIN_ID_ANIMATION_ROTATE)->Render(position.x, position.y, GetLayer(layer, orderInLayer));
    else
        sprites->Get(COIN_ID_SPRITE_NORMAL)->Draw(position.x, position.y, GetLayer(layer, orderInLayer));

}