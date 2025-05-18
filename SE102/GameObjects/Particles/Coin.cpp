#include "Coin.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/Coin.h"
#include "ScorePopup.h"
#include "GameObjects/Blocks/Brick.h"

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
        velocity.y -= COIN_INIT_Y_VEL;
        killTimer = COIN_KILL_TIME;
    }
    break;
    }
}

void CCoin::OnDelete()
{
    if (!noScore) {
        CGame* const game = CGame::GetInstance();
        game->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y));
    }
    if (brick != NULL) {
        brick->Delete();
    }
}

void CCoin::SwitchToBrick()
{
    state = COIN_STATE_BRICK;
    brick = new CBrick(position.x, position.y);
    CGame::GetInstance()->GetCurrentScene()->AddObject(brick);
}

void CCoin::SwitchToCoin()
{
    if (brick == NULL)
        return;
    brick->Delete();
    brick = NULL;
}

void CCoin::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state != COIN_STATE_INTRO)
        return;

    velocity.y = min(velocity.y + OBJECT_FALL * 2.0f, OBJECT_MAX_FALL * 2.0f);
    position.y += velocity.y * dt;

    //if (velocity.y >= OBJECT_MAX_FALL * 1.0f)
    //    Delete();

    if (killTimer > 0) killTimer -= dt;
    else {
        Delete();
    }
}

void CCoin::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    if (state == COIN_STATE_INTRO)
        animations->Get(COIN_ID_ANIMATION_INTRO)->Render(position.x, position.y, GetLayer(layer, orderInLayer));
    else
        animations->Get(COIN_ID_ANIMATION_NORMAL)->Render(position.x, position.y, GetLayer(layer, orderInLayer));

}