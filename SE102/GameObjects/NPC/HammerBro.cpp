#include "HammerBro.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/HammerBro.h"
#include <iostream>
#include <algorithm>
#include <Engine/Helper.h>

#include "Goomba.h"
#include "Piranha.h"
#include "GameObjects/Blocks/QuestionBlock.h"
#include "GameObjects/Blocks/Brick.h"

#include "GameObjects/Particles/ScorePopup.h"
#include "GameObjects/NPC/Boomerang.h"
#include "ContentIds/Boomerang.h"
#include <ContentIds/Wing.h>

using namespace std;

bool CHammerBro::IsContentLoaded = false;

void CHammerBro::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(HAMMER_BRO_SPRITES_PATH);
    loader.Load(BOOMERANG_SPRITES_PATH);
}

CHammerBro::CHammerBro(float x, float y) : CGameObject(x, y, 0.0f)
{
    LoadContent();
    layer = SortingLayer::NPC;
    SetState(HAMMER_BRO_STATE_NORMAL);
    nx = 1;
    player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
}

void CHammerBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = position.x - 3;
    top = position.y + - 8;
    right = position.x + 3;
    bottom = position.y + 15;
}

void CHammerBro::SetState(int state)
{
    switch (state)
    {
    case HAMMER_BRO_STATE_DEAD_BOUNCE:
    {
        CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, ScoreCombo));
        layer = SortingLayer::CORPSE;
        velocity.y = -OBJECT_DEAD_BOUNCE;
        velocity.x = OBJECT_DEAD_X_VEL;
    }
    break;
    case HAMMER_BRO_STATE_DEAD:
    {
        CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, ScoreCombo));
        layer = SortingLayer::CORPSE;
        velocity.y = -OBJECT_DEAD_BOUNCE;
        //position.y += 16.0f;
        velocity.x = 0.0f;
    }
    break;
    default: 
        changeDirTimer = HAMMER_BRO_CHANGE_DIR_TIME;
        break;
    }
    this->state = state;
}

void CHammerBro::OnNoCollision(float dt)
{
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    onGround = false;
}

void CHammerBro::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->nx != 0 && e->obj->IsBlocking())
    {
        nx *= -1;
    }
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        velocity.y = 0.0f;
        if (e->ny < 0)
            onGround = true;
    }
}

void CHammerBro::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    constexpr float floaty = -0x01000 * SUBSUBSUBPIXEL_DELTA_TIME;
    velocity.y = min(velocity.y + OBJECT_FALL * 0.8f, OBJECT_MAX_FALL + floaty);

    if (stopTimer > 0)
        stopTimer -= dt;
    else {
        if (changeDirTimer > 0)
            changeDirTimer -= dt;
        else {
            stopTimer = HAMMER_BRO_STOP_TIME;
            changeDirCount++;
            changeDirTimer = HAMMER_BRO_CHANGE_DIR_TIME;
            nx *= -1;
        }
    }

    cout << changeDirTimer << '\n';
    switch (state)
    {
    case HAMMER_BRO_STATE_NORMAL:
    {
        if (stopTimer <= 0) {
            velocity.x = HAMMER_BRO_X_SPEED * (nx > 0 ? 1 : -1);
            if (onGround && changeDirCount >= 4)
            {
                changeDirCount = 0;
                velocity.y = -HAMMER_BRO_WING_BIG_HOP;
            }
        }
        else
            velocity.x = 0.0f;

        if (resetThrownTimer > 0)
            resetThrownTimer -= dt;
        else {
            thrown = false;
        }

        if (throwTimer > 0)
            throwTimer -= dt;
        else {
            throwTimer = HAMMER_BRO_TIMER_RELOAD;
            auto scene = CGame::GetInstance()->GetCurrentScene();
            if (player != NULL) {
                int flipX = sign(player->GetPosition().x - position.x);
                scene->AddObject(new CBoomerang(position.x, position.y - 5.0f, flipX, this));
                thrown = true;
                resetThrownTimer = HAMMER_BRO_RESET_THROW;
            }
        }
    }
    break;
    case HAMMER_BRO_STATE_DEAD_BOUNCE: break;
    case HAMMER_BRO_STATE_DEAD: break;
    }

    //if (player != NULL)
    //{
    //    nx = player->GetNx();
    //    position = player->GetPosition() + Vector2(12.0f * nx, holdYOffset);
    //    velocity.x = 0;
    //    velocity.y = 0;
    //}

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CHammerBro::Render() {
    CAnimations* const animations = CAnimations::GetInstance();
    CSprites* const sprites = CSprites::GetInstance();

    bool flipX = false;
    if (player != NULL && state == HAMMER_BRO_STATE_NORMAL) {
        flipX = sign(player->GetPosition().x - position.x) > 0 ? true : false;
    }

    CAnimation* animation;

    if (thrown) {
        animation = animations->Get(HAMMER_BRO_ID_ANIMATION_WALK);
    }
    else {
        animation = animations->Get(HAMMER_BRO_ID_ANIMATION_WALKTHROW);
        sprites->Get(BOOMERANG_ID_SPRITE_BOOMERANG_FRAME_2)->Draw(position.x + 10.0f * (flipX ? -1 : 1), position.y - 5.0f, 0.0f, flipX);
    }
    if (abs(velocity.x) > 0)
        animation->Play();
    else
        animation->Stop();
    animation->Render(position.x, position.y + (state == HAMMER_BRO_STATE_DEAD ? 16.0f : 0.0f), GetLayer(layer, orderInLayer), flipX, (state == HAMMER_BRO_STATE_DEAD));
    //RenderBoundingBox();
}