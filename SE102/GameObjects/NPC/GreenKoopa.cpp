#include "GreenKoopa.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/GreenKoopa.h"
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

bool CGreenKoopa::IsContentLoaded = false;

void CGreenKoopa::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(WING_SPRITES_PATH);
    loader.Load(GREEN_KOOPA_SPRITES_PATH);
}

int CGreenKoopa::GetAnimationId()
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

void CGreenKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = position.x - 3;
    top = position.y + 4;
    right = position.x + 3;
    bottom = position.y + 16;
}

void CGreenKoopa::SetState(int state)
{
    switch (state)
    {
    case KOOPA_STATE_DEAD_BOUNCE:
    {
        if (ignoreDamageTimer > 0) return;
        CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y));
        if (this->state == KOOPA_STATE_WING)
        {
            state = KOOPA_STATE_NORMAL;
            velocity.x = 0.0f;
            ignoreDamageTimer = GOOMA_IGNORE_DAMAGE_TIME;
        }
        else
        {
            CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, Score400));
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

void CGreenKoopa::PlayerHit(int nx)
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

void CGreenKoopa::AttachHold(LPGAMEOBJECT player, float holdYOffset)
{
    if (this->player != NULL) return;
    this->player = player;
    this->holdYOffset = holdYOffset;
}

void CGreenKoopa::DetachHold()
{
    if (this->player == NULL) return;
    this->player = NULL;
    position.y += holdYOffset / 2.0f;
    holdYOffset = 0;

}

void CGreenKoopa::OnNoCollision(float dt)
{
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    onGround = false;
}

void CGreenKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->nx != 0 && e->obj->IsBlocking())
    {
        nx *= -1;
    }
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        if (state == KOOPA_STATE_WING && e->ny > 0)
            velocity.y = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;
        else
           velocity.y = 0.0f;
        if (e->ny < 0)
            onGround = true;
    }
    
    if (state == KOOPA_STATE_IN_SHELL && abs(velocity.x) > 0)
    {
        if (dynamic_cast<CGoomba*>(e->obj))
        {
            CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
            goomba->DeadBounce();
        }
        else if (dynamic_cast<CGreenKoopa*>(e->obj))
        {
            CGreenKoopa* koopa = dynamic_cast<CGreenKoopa*>(e->obj);
            koopa->DeadBounce();
        }
        if (dynamic_cast<CQuestionBlock*>(e->obj) && e->ny == 0)
        {
            CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
            questionBlock->Hit(1);
        }
        // YEP this has to be checked at two different code i dont know why but it works
        // This is the bug where the shell decided that it would hit brick that it wasn't supposed to
        else if (dynamic_cast<CBrick*>(e->obj) && e->ny == 0)
        {
            CBrick* brick = dynamic_cast<CBrick*>(e->obj);
            brick->Hit();
        }
    }
}

void CGreenKoopa::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    if (ignoreDamageTimer > 0) ignoreDamageTimer -= dt;

    if (state == KOOPA_STATE_WING)
    {
        constexpr float greenShellY = -0x01000 * SUBSUBSUBPIXEL_DELTA_TIME; // Green koopa is floaty
        velocity.y = min(velocity.y + OBJECT_FALL, OBJECT_MAX_FALL) + greenShellY;
    }
    else if (state == KOOPA_STATE_IN_SHELL)
    {
        constexpr float ShellY = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME; // Green koopa is floaty
        velocity.y = min(velocity.y + OBJECT_FALL, OBJECT_MAX_FALL) + ShellY;

    }
    else
        velocity.y = min(velocity.y + OBJECT_FALL, OBJECT_MAX_FALL);
    switch (state)
    {
    case KOOPA_STATE_WING:
    {
        velocity.x = GREEN_KOOPA_X_SPEED * (nx > 0 ? 1 : -1);

        if (hopTimer > 0) hopTimer -= dt;
        if (wingActivateTimer > 0) wingActivateTimer -= dt;
        if (changeDirTimer > 0) changeDirTimer -= dt;

        if (onGround)
        {
            if (wingActivateTimer <= 0)
            {
                if (hopTimer <= 0)
                {
                    hopTimer = KOOPA_WING_HOP_TIME;
                    hopCount = (hopCount + 1);
                    velocity.y = -KOOPA_WING_HOP;
                    if (hopCount == 4)
                    {
                        velocity.y = -KOOPA_WING_BIG_HOP;
                        wingActivateTimer = KOOPA_WING_ACTIVATE_TIME;
                        hopCount = 0;
                    }
                }
            }
            // Change Goomba direction to player
            else if (changeDirTimer <= 0)
            {
                //LPGAMEOBJECT player = CGame::GetInstance()->GetCurrentScene()->GetPlayer();
                //if (player != NULL)
                //{
                    //nx = sign(player->GetPosition().x - position.x);
                //} 
            }
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
    }
    break;
    case KOOPA_STATE_RESPAWNING:
    {

        if (respawnTimer > 0) respawnTimer -= dt;
        else
        {
            DetachHold();
            nx = -1;
            respawnTimer = KOOPA_RESPAWNING_TIME;
            state = KOOPA_STATE_NORMAL;
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

void CGreenKoopa::Render() {
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
    RenderBoundingBox();
}