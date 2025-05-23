#include "Koopa.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/GreenKoopa.h"
#include "ContentIds/RedKoopa.h"
#include <iostream>
#include <algorithm>
#include <Engine/Helper.h>

#include "Goomba.h"
#include "Piranha.h"
#include "GameObjects/Blocks/QuestionBlock.h"
#include "GameObjects/Blocks/Brick.h"

#include "GameObjects/Particles/ScorePopup.h"
#include <ContentIds/Wing.h>

using namespace std;

bool CKoopa::IsContentLoaded = false;

void CKoopa::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(WING_SPRITES_PATH);
    loader.Load(GREEN_KOOPA_SPRITES_PATH);
    loader.Load(RED_KOOPA_SPRITES_PATH);
}

int CKoopa::GetAnimationGreenId()
{
    if (state == KOOPA_STATE_RESPAWNING)
        return GREEN_KOOPA_ID_ANIMATION_RESPAWN;
    if (state == KOOPA_STATE_NORMAL || state == KOOPA_STATE_WING)
        return GREEN_KOOPA_ID_ANIMATION_WALK;
    else
    {
        if (state == KOOPA_STATE_DEAD_BOUNCE)
            return GREEN_KOOPA_ID_ANIMATION_SHELL_IDLE;
        if (abs(velocity.x) > 0)
            return GREEN_KOOPA_ID_ANIMATION_SHELL;
        return GREEN_KOOPA_ID_ANIMATION_SHELL_IDLE;
    }
}

int CKoopa::GetAnimationId()
{
    if (isRed)
        return GetAnimationRedId();
    return GetAnimationGreenId();
}

int CKoopa::GetAnimationRedId()
{
    if (state == KOOPA_STATE_RESPAWNING)
        return RED_KOOPA_ID_ANIMATION_RESPAWN;
    if (state == KOOPA_STATE_NORMAL)
        return RED_KOOPA_ID_ANIMATION_WALK;
    else
    {
        if (state == KOOPA_STATE_DEAD_BOUNCE)
            return RED_KOOPA_ID_ANIMATION_SHELL_IDLE;
        if (abs(velocity.x) > 0)
            return RED_KOOPA_ID_ANIMATION_SHELL;
        return RED_KOOPA_ID_ANIMATION_SHELL_IDLE;
    }
}

CKoopa::CKoopa(float x, float y) : CGameObject(x, y, 0.0f)
{
    LoadContent();
    nx = -1;
    shellCast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return dynamic_cast<CBrick*>(obj) != nullptr;
        });
    turnCast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return !(obj == this || !obj->IsBlocking());
        });
    layer = SortingLayer::NPC;
    SetState(KOOPA_STATE_NORMAL);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == KOOPA_STATE_IN_SHELL)
    {
        left = position.x - 4;
        top = position.y + 5;
        right = position.x + 4;
        bottom = position.y + 16;
    }
    else
    {
        left = position.x - 3;
        top = position.y + 4;
        right = position.x + 3;
        bottom = position.y + 16;
    }
}

void CKoopa::SetState(int state)
{
    switch (state)
    {
    case KOOPA_STATE_DEAD_BOUNCE:
    {
        if (ignoreDamageTimer > 0) return;
        if (this->state == KOOPA_STATE_WING)
        {
            state = KOOPA_STATE_NORMAL;
            velocity.x = 0.0f;
            ignoreDamageTimer = GOOMA_IGNORE_DAMAGE_TIME;
        }
        else
        {
            CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, ScoreCombo));
            layer = SortingLayer::CORPSE;
            velocity.y = -OBJECT_DEAD_BOUNCE;
            velocity.x = OBJECT_DEAD_X_VEL;
        }
    }
    break;
    default: break;
    }
    this->state = state;
}

void CKoopa::PlayerHit(int nx)
{
    if (state != KOOPA_STATE_IN_SHELL)
    {
        velocity.x = 0;

        if (this->state == KOOPA_STATE_WING)
            state = KOOPA_STATE_NORMAL;
        else
            state = KOOPA_STATE_IN_SHELL;
        respawnTimer = GREEN_KOOPA_SPAWN_TIME;
        this->nx = 0;
    }
    else
    {
        if (velocity.x != 0)
            this->nx = 0;
        else
            this->nx = -nx;
    }
}

void CKoopa::AttachHold(LPGAMEOBJECT player, float holdYOffset)
{
    if (this->player != NULL) return;
    this->player = player;
    this->holdYOffset = holdYOffset;
}

void CKoopa::DetachHold()
{
    if (this->player == NULL) return;
    this->player = NULL;
    position.y += holdYOffset / 2.0f;
    holdYOffset = 0;

}

void CKoopa::OnNoCollision(float dt)
{
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    onGround = false;
}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
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
    
    if (state == KOOPA_STATE_IN_SHELL && abs(velocity.x) > 0.0f)
    {
        if (dynamic_cast<CGoomba*>(e->obj))
        {
            CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
            goomba->DeadBounce();
        }
        else if (dynamic_cast<CKoopa*>(e->obj))
        {
            CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
            koopa->DeadBounce();
        }
        if (dynamic_cast<CQuestionBlock*>(e->obj) && e->ny == 0)
        {
            CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
            questionBlock->Hit(1);
        }
        // YEP this has to be checked at two different code i dont know why but it works
        // This is the bug where the shell decided that it would hit brick that it wasn't supposed to
        //else if (dynamic_cast<CBrick*>(e->obj) && e->ny == 0)
        //{
        //    CBrick* brick = dynamic_cast<CBrick*>(e->obj);
        //    brick->Hit();
        //}
    }
}

void CKoopa::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    if (state == KOOPA_STATE_NORMAL && isRed)
    {
        turnCast.SetBoundingBox(position + Vector2((7.0f) * nx, 16.0f), Vector2(8.0f, 6.0f));
        turnCast.CheckOverlap(coObjects);
        if (turnCast.collision.size() <= 0)
        {
            nx *= -1;
        }
    }

    if (ignoreDamageTimer > 0) ignoreDamageTimer -= dt;

    if (state == KOOPA_STATE_WING)
    {
        constexpr float greenShellY = -0x01000 * SUBSUBSUBPIXEL_DELTA_TIME; // Green koopa is floaty
        velocity.y = min(velocity.y + OBJECT_FALL, OBJECT_MAX_FALL + greenShellY);
    }
    else if (state == KOOPA_STATE_IN_SHELL)
    {
        //constexpr float ShellY = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME; // Green koopa is floaty
        velocity.y = min(velocity.y + OBJECT_FALL, OBJECT_MAX_FALL);

    }
    else
        velocity.y = min(velocity.y + OBJECT_FALL, OBJECT_MAX_FALL);

    switch (state)
    {
    case KOOPA_STATE_WING:
    {
        velocity.x = GREEN_KOOPA_X_SPEED * (nx > 0 ? 1 : -1);


        if (onGround)
        {
            velocity.y = -KOOPA_WING_BIG_HOP;
        }

    }
    break;
    case KOOPA_STATE_NORMAL:
    {
        velocity.x = GREEN_KOOPA_X_SPEED * (nx > 0 ? 1 : -1);
    }
    break;
    case KOOPA_STATE_IN_SHELL:
    {
        if (player != NULL)
        {
            nx = player->GetNx();
            position = player->GetPosition() + Vector2(12.0f * nx, holdYOffset);
            velocity.x = 0;
            velocity.y = 0;
        }
        else
        {
            velocity.x = GREEN_KOOPA_SHELL_X_SPEED * nx;
        }
        if (velocity.x == 0)
        {
            if (respawnTimer > 0) respawnTimer -= dt;
            else
            {
                respawnTimer = KOOPA_RESPAWNING_TIME;
                state = KOOPA_STATE_RESPAWNING;
            }
        }
        shellCast.SetBoundingBox(position + Vector2(8.0f * (nx > 0 ? 1 : -1), +10.0f), Vector2(3.0f, 3.0f));
        shellCast.CheckOverlap(coObjects);
        if (shellCast.collision.size() >= 1 && abs(velocity.x) > 0)
        {
            CBrick* brick = dynamic_cast<CBrick*>(shellCast.collision[0]);
            if (brick != NULL)
            {
                brick->Hit();
                nx *= -1;
            }
        }
    }
    break;
    case KOOPA_STATE_RESPAWNING:
    {
        if (respawnTimer > 0) respawnTimer -= dt;
        else
        {
            DetachHold();
            nx = -1;
            insideWallCast.SetBoundingBox(position, Vector2(5.0f, 5.0f));
            insideWallCast.CheckOverlap(coObjects);
            if (insideWallCast.collision.size() > 0)
            {
                SetState(KOOPA_STATE_DEAD_BOUNCE);
            }
            else
            {
                respawnTimer = KOOPA_RESPAWNING_TIME;
                state = KOOPA_STATE_NORMAL;
            }
        }
    }
    break;
    case KOOPA_STATE_DEAD_BOUNCE: break;
    }

    if (player != NULL)
    {
        nx = player->GetNx();
        position = player->GetPosition() + Vector2(12.0f * nx, holdYOffset);
        velocity.x = 0;
        velocity.y = 0;
    }

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render() {
    CAnimations* const animations = CAnimations::GetInstance();


    if (state == KOOPA_STATE_WING)
    {
        Vector2 wingOffset = Vector2(2.0f * (nx > 0 ? -1 : 1), 7.0f);
        if (wingActivateTimer <= 0.0f || (wingActivateTimer >= 0.0f && !onGround))
        {
            animations->Get(WING_ID_ANIMATION_WING_ACTIVE)->Render(position.x + wingOffset.x, position.y - wingOffset.y, GetLayer(layer, orderInLayer), nx > 0);
        }
        else
        {
            animations->Get(WING_ID_ANIMATION_WING_IDLE)->Render(position.x + wingOffset.x, position.y - wingOffset.y, GetLayer(layer, orderInLayer), nx > 0);
        }
    }

    switch (state)
    {
    case KOOPA_STATE_WING:
    case KOOPA_STATE_NORMAL:
        {
            animations->Get(GetAnimationId())->Render(position.x, position.y - 0, GetLayer(layer, orderInLayer), nx > 0);
        }
        break;
    case KOOPA_STATE_IN_SHELL:
        {
            auto animation = animations->Get(GetAnimationId());
            animation->Render(position.x, position.y + 8, GetLayer(layer, orderInLayer), false, false);
        }
        break;
    case KOOPA_STATE_DEAD_BOUNCE:
        {
            auto animation = animations->Get(GetAnimationId());
            animation->Render(position.x, position.y + 8, GetLayer(layer, orderInLayer), nx > 0, true);
        }
        break;
    case KOOPA_STATE_RESPAWNING:
        {
            auto animation = animations->Get(GetAnimationId());
            if (GetTickCount64() % 3 == 0)
                respawnNx = (respawnNx + 1) % 2;
            animation->Render(position.x + (respawnNx - 1) * 1, position.y + 8, GetLayer(layer, orderInLayer), nx > 0, false);
        }
        break;
    }
    //RenderBoundingBox();
    shellCast.RenderBoundingBox();
}