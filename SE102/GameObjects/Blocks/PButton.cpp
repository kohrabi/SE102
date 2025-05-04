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
    SetState(P_BUTTON_STATE_INACTIVE);
    CPlayScene* currentScene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    currentScene->GetCamera()->Shake();
    const vector<LPGAMEOBJECT>& objects = currentScene->GetObjectList();
    for (int i = 0; i < objects.size(); i++)
    {
        LPGAMEOBJECT object = objects[i];
        if (object == NULL || object->IsDeleted()) continue;
        if (dynamic_cast<CBrick*>(object) != nullptr && object->IsColliderInCamera())
        {
            currentScene->AddObject(new CCoin(object->GetPosition().x, object->GetPosition().y));
            object->Delete();
        }
    }
}
