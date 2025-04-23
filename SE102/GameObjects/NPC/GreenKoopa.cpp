#include "GreenKoopa.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/GreenKoopa.h"
#include <iostream>
#include <algorithm>
#include <Engine/Helper.h>

#include "Goomba.h"
#include "FirePiranha.h"
#include "GameObjects/QuestionBlock.h"

#include "GameObjects/ScorePopup.h"

using namespace std;

bool CGreenKoopa::IsContentLoaded = false;

void CGreenKoopa::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(GREEN_KOOPA_SPRITES_PATH);
}

int CGreenKoopa::GetAnimationId()
{
    if (state == KOOPA_STATE_RESPAWNING)
        return GREEN_KOOPA_ID_ANIMATION_RESPAWN;
    if (state == KOOPA_STATE_NORMAL)
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

void CGreenKoopa::SetState(int state)
{
    switch (state)
    {
    case KOOPA_STATE_NORMAL:
    {

    }
    break;
    case KOOPA_STATE_RESPAWNING:
    {

    }
    break;
    case KOOPA_STATE_DEAD_BOUNCE:
    {
        CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, Score400));
        layer = SortingLayer::CORPSE;
        velocity.y = -OBJECT_DEAD_BOUNCE;
        velocity.x = OBJECT_DEAD_X_VEL;
    }
    break;
    }
    this->state = state;
}

void CGreenKoopa::PlayerHit(int nx)
{
    if (state != KOOPA_STATE_IN_SHELL)
    {
        velocity.x = 0;
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
        cout << -nx << " " << this->nx << '\n';
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
}

void CGreenKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->nx != 0 && e->obj->IsBlocking())
    {
        nx *= -1;
    }
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        velocity.y = 0.0f;
    }
    
    if (state == KOOPA_STATE_IN_SHELL)
    {
        if (dynamic_cast<CGoomba*>(e->obj))
        {
            CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
            goomba->DeadBounce();
        }
        if (dynamic_cast<CGreenKoopa*>(e->obj))
        {
            CGreenKoopa* koopa = dynamic_cast<CGreenKoopa*>(e->obj);
            koopa->DeadBounce();
        }
        if (dynamic_cast<CQuestionBlock*>(e->obj) && e->ny == 0)
        {
            CQuestionBlock* questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
            questionBlock->Hit(1);
        }
    }
}

void CGreenKoopa::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    velocity.y += OBJECT_FALL;
    velocity.y = min(velocity.y, OBJECT_MAX_FALL);

    switch (state)
    {
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

    switch (state)
    {
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
}