#include "Coin.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "contents.h"

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


void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    position.y -= COIN_Y_VELOCITY * dt;

    position.y = max(position.y, maxYPos);
    DWORD now = GetTickCount64();
    if (now > startTime) {
        Delete();
        cout << "delete\n";
    }
}

void CCoin::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    animations->Get(COIN_ID_ANIMATION_NORMAL)->Render(position.x, position.y);
}