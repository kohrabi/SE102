#include "PButton.h"

#include "GameObjects/Mario.h"

#include "Engine/Graphics/Animations.h"
#include "Engine/Graphics/Sprites.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "GameObjects/Particles/Coin.h"
#include "GameObjects/Powerups/Mushroom.h"
#include "GameObjects/Powerups/Leaf.h"

#include "ContentIds/PButton.h"

#include "GameObjects/NPC/Goomba.h"
#include "GameObjects/NPC/Koopa.h"

#include "GameObjects/Powerups/OneUp.h"
#include "ContentIds/Particles.h"

#include <iostream>
#include "Brick.h"
#include <Engine/PlayScene.h>
using namespace std;

bool CPButton::IsContentLoaded = false;

void CPButton::LoadContent()
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(P_BUTTON_SPRITES_PATH);
    loader.Load(PARTICLES_SPRITES_PATH);
}


CPButton::CPButton(float x, float y)
    : CGameObject(x, y, 0.0f)
{
    LoadContent();
    puffTimer = PUFF_TIME;
    LPANIMATION puff = CAnimations::GetInstance()->Get(PARTICLES_ID_ANIMATION_PUFF);
    puff->Reset();
    puff->SetLoop(false);
    layer = SortingLayer::BLOCK;
    SetState(P_BUTTON_STATE_ACTIVE);
}

void CPButton::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    puffTimer -= dt;
    if (switchBackTimer > 0) switchBackTimer -= dt;
    else if (switched) {
        CPlayScene* currentScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        const vector<LPGAMEOBJECT>& objects = currentScene->GetObjectList();
        int size = objects.size();

        for (int i = 0; i < size; i++) {
            LPGAMEOBJECT object = objects[i];
            if (object == NULL || object->IsDeleted() || !object->IsColliderInCamera()) continue;
            if (object->GetState() == BRICK_STATE_COIN && dynamic_cast<CBrick*>(object) != nullptr)
                dynamic_cast<CBrick*>(object)->SwitchToBrick();
            if (object->GetState() == COIN_STATE_BRICK && dynamic_cast<CCoin*>(object) != nullptr)
                dynamic_cast<CCoin*>(object)->SwitchToCoin();
        }

        switched = false;
    }
}

void CPButton::Render()
{
    CAnimations* const animations = CAnimations::GetInstance();
    animations->Get(state == P_BUTTON_STATE_ACTIVE ? P_BUTTON_ID_ANIMATION_ACTIVE : P_BUTTON_ID_ANIMATION_INACTIVE)->Render(position.x, position.y, 1.0f);
    if (puffTimer > 0)
        animations->Get(PARTICLES_ID_ANIMATION_PUFF)->Render(position.x, position.y, 1.0f);
}

void CPButton::SetState(int state)
{
    if (this->state == state) return;
    this->state = state;
}

void CPButton::Hit()
{
    if (state == P_BUTTON_STATE_INACTIVE)
        return;
    SetState(P_BUTTON_STATE_INACTIVE);
    CPlayScene* currentScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    currentScene->GetCamera()->Shake();
    const vector<LPGAMEOBJECT>& objects = currentScene->GetObjectList();
    int size = objects.size();
    for (int i = 0; i < size; i++)
    {
        LPGAMEOBJECT object = objects[i];
        if (object == NULL || object->IsDeleted() || !object->IsColliderInCamera()) continue;
        if (dynamic_cast<CBrick*>(object) != nullptr)
            dynamic_cast<CBrick*>(object)->SwitchToCoin();
        else if (dynamic_cast<CCoin*>(object) != nullptr)
            dynamic_cast<CCoin*>(object)->SwitchToBrick();
    }
    switched = true;
    switchBackTimer = SWITCH_BACK_TIME;
}
