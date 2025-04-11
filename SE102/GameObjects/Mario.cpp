#include "Mario.h"
#include "Engine/Loaders/SpritesLoader.h"

#include "Engine/Game.h"
#include "Engine/Graphics/Animations.h"

#include "ContentIds/Mario.h"
#include "ContentIds/MarioBig.h"

#include "Engine/Helper.h"
#include <Engine/debug.h>

#include "QuestionBlock.h"
#include "Coin.h"
#include "NPC/Goomba.h"
#include "NPC/GreenKoopa.h"
#include "NPC/Mushroom.h"

#include <iostream>
using namespace std;

bool CMario::IsContentLoaded = false;

void CMario::LoadContent() 
{
    if (IsContentLoaded)
        return;
    CMario::IsContentLoaded = true;

    SpritesLoader loader;
    loader.Load(MARIO_SPRITES_PATH);
    loader.Load(MARIO_BIG_SPRITES_PATH);
}

CMario::CMario(float x, float y) : CGameObject(x, y, 0.0f)
{
    LoadContent();
    nx = 1;
    cast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return dynamic_cast<CGreenKoopa*>(obj) != nullptr;
    });
    layer = SortingLayer::MARIO;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    CGame* const game = CGame::GetInstance();
    float dts = dt / 1000.f;
    
    // Holding Shell
    isHolding = false;
    if (game->IsKeyDown(KEY_RUN))
    {
        if (powerUp == MARIO_POWERUP_SMALL)
            cast.SetBoundingBox(position + Vector2((10) * nx, 0), Vector2(4, 6));
        else
            cast.SetBoundingBox(position + Vector2((10) * nx, 8), Vector2(4, 6));

        cast.CheckOverlap(coObjects);
        if (cast.collision.size() > 0)
        {
            isHolding = true;
            if (holdShell == NULL)
            {
                holdShell = dynamic_cast<CGreenKoopa*>(cast.collision[0]);
                if (holdShell->IsInShell())
                {
                    if (powerUp == MARIO_POWERUP_SMALL)
                        holdShell->AttachHold(this, -8);
                    else
                        holdShell->AttachHold(this, 0);
                }
                else
                    isHolding = false;
            }
        }
    }

    if (!isHolding && holdShell != NULL)
    {
        holdShell->DetachHold();
        holdShell = NULL;
    }

    // X Movement
    accel.x = 0;

    bool run = false;
    if (game->IsKeyDown(KEY_RUN))
        run = true;
    if (game->IsKeyJustReleased(KEY_RUN)) {
        runBeforeWalkTimer = RUN_TIME_BEFORE_WALK;
    }

    if (runBeforeWalkTimer > 0)
        runBeforeWalkTimer -= dt;

    if (game->IsKeyDown(KEY_MOVE_LEFT)) {
        accel.x = -(run ? RUNNING_ACCELERATION : WALKING_ACCELERATION);
        nx = -1;
    }
    if (game->IsKeyDown(KEY_MOVE_RIGHT)) {
        accel.x = (run ? RUNNING_ACCELERATION : WALKING_ACCELERATION);
        nx = 1;
    }

    if (accel.x != 0.0f) {
        if (velocity.x == 0.0f)
            accel.x = sign(accel.x) * MINIMUM_WALK_VELOCITY;
    }

    skidding = false;
    if (accel.x == 0.0f) {
        velocity.x = move_towards(velocity.x, 0, RELEASE_DECELERATION);
    }
    else if (sign(accel.x) != sign(velocity.x) && velocity.x != 0) {
        skidding = true;
        velocity.x = move_towards(velocity.x, 0, SKIDDING_DECELERATION);
    }

    if (run || (abs(velocity.x) > MAXIMUM_WALK_SPEED + WALKING_ACCELERATION && runBeforeWalkTimer > 0)) 
        velocity.x = clampf(velocity.x, -MAXIMUM_RUNNING_SPEED, MAXIMUM_RUNNING_SPEED);
    else
        velocity.x = clampf(velocity.x, -MAXIMUM_WALK_SPEED, MAXIMUM_WALK_SPEED);

    // Y MOVEMENT
    float gravity = 0.0f;
    if (game->IsKeyDown(KEY_JUMP) && velocity.y < JUMP_MAX_NEGATIVE)
    {
        gravity = JUMP_HELD_GRAVITY;
    }
    else
    {
        gravity = JUMP_GRAVITY;
    } 
    accel.y = gravity;
    
    if (game->IsKeyJustPressed(KEY_JUMP) && isOnGround) {
        float initVel = -JUMP_INIT_VEL;
        int temp = trunc(abs(velocity.x) * MAX_DELTA_TIME);
        if (temp == 0)
            initVel -= 0x00000 * SUBSUBSUBPIXEL_DELTA_TIME;
        else if (temp == 1)
            initVel -= 0x00200 * SUBSUBSUBPIXEL_DELTA_TIME;
        else if (temp == 2)
            initVel -= 0x00400 * SUBSUBSUBPIXEL_DELTA_TIME;
        else if (temp == 3)
            initVel -= 0x00800 * SUBSUBSUBPIXEL_DELTA_TIME;
        accel.y = initVel;
    }

    velocity.x += accel.x;
    velocity.y += accel.y;

    velocity.y = min(velocity.y, MAX_FALL_SPEED);


    //cout << velocity.y << '\n';

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    LPANIMATION animation = animations->Get(GetAnimationID());
    if (abs(velocity.x) <= MAXIMUM_WALK_SPEED + WALKING_ACCELERATION)
        animation->SetTimeScale(1.0f);
    else
        animation->SetTimeScale(0.5f);

    bool flipX = nx > 0 ? true : false;
    animation->Render(position.x, position.y, GetLayer(layer, orderInLayer), flipX);
    RenderBoundingBox();
    cast.RenderBoundingBox();
}

int CMario::GetAnimationIDSmall()
{
    if (isHolding)
    {
        if (abs(velocity.x) > 0)
            return MARIO_ID_ANIMATION_HOLD_WALK;
        else
            return MARIO_ID_ANIMATION_HOLD_IDLE;
    }
    if (!isOnGround)
        return MARIO_ID_ANIMATION_JUMP;
    if (skidding)
        return MARIO_ID_ANIMATION_SKIDDING;
    if (abs(velocity.x) > 0)
        return MARIO_ID_ANIMATION_WALK;
    else
        return MARIO_ID_ANIMATION_IDLE;
}

int CMario::GetAnimationIDBig()
{
    if (isHolding)
    {
        if (abs(velocity.x) > 0)
            return MARIO_BIG_ID_ANIMATION_HOLD_WALK;
        else            
            return MARIO_BIG_ID_ANIMATION_HOLD_IDLE;
    }
    if (!isOnGround)
        return MARIO_BIG_ID_ANIMATION_JUMP;
    if (skidding)
        return MARIO_BIG_ID_ANIMATION_SKIDDING;
    if (abs(velocity.x) > 0)
        return MARIO_BIG_ID_ANIMATION_WALK;
    else
        return MARIO_BIG_ID_ANIMATION_IDLE;
}

int CMario::GetAnimationID()
{
    switch (powerUp)
    {
    case MARIO_POWERUP_SMALL: return GetAnimationIDSmall();
    case MARIO_POWERUP_BIG: return GetAnimationIDBig();
    }
    return 0;
}

void CMario::SetState(int state) {
    this->state = state;
}

void CMario::OnNoCollision(DWORD dt) {
    position.x += velocity.x * dt;
    position.y += velocity.y * dt;
    isOnGround = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e) {
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		velocity.y = 0;
        if (e->ny < 0) isOnGround = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		velocity.x = 0;
	}
    
    if (dynamic_cast<CQuestionBlock*>(e->obj) && e->ny > 0) 
    {
        CQuestionBlock* const questionBlock = dynamic_cast<CQuestionBlock*>(e->obj);
        questionBlock->Hit();
    }
    else if (dynamic_cast<CCoin*>(e->obj) && e->obj->GetState() == COIN_STATE_NORMAL)
    {
        e->obj->Delete();
        cout<<"AddCoin\n";
    }
    else if (dynamic_cast<CGoomba*>(e->obj))
    {
        CGoomba* const goomba = dynamic_cast<CGoomba*>(e->obj);
        if (e->ny >= 0)
        {
            cout << "ouch!!!\n";
        }
        else
        {
            goomba->SetKill();
            velocity.y = -JUMP_INIT_VEL;
        }
    }
    else if (dynamic_cast<CMushroom*>(e->obj))
    {
        e->obj->Delete();
        powerUp = MARIO_POWERUP_BIG;
    }
    else if (dynamic_cast<CGreenKoopa*>(e->obj))
    {
        CGreenKoopa* const koopa = dynamic_cast<CGreenKoopa*>(e->obj);
        if (e->ny >= 0)
        {
            if (koopa->IsInShell() && koopa->GetVelocity().x == 0.0f)
            {
                koopa->SetNx(nx);
            }
            cout << "ouch!!!\n";
        }
        else
        {
            //if (abs(position.x - koopa->GetPosition().x) <= 4)
            //    koopa->PlayerHit(0);
            //else    
                koopa->PlayerHit(sign(position.x - koopa->GetPosition().x));
            velocity.y = -JUMP_INIT_VEL;
        }
    }
}