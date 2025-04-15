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
#include "ScorePopup.h"

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
    SetState(MARIO_STATE_NORMAL);
}

void CMario::marioNormalUpdate(float dt, vector<LPGAMEOBJECT>* coObjects)
{

    CGame* const game = CGame::GetInstance();
    // Holding Shell
    isHolding = false;
    if (game->IsKeyDown(KEY_RUN))
    {
        if (powerUp == MARIO_POWERUP_SMALL)
            cast.SetBoundingBox(position + Vector2((10.0f) * nx, 0.0f), Vector2(4.0f, 6.0f));
        else
            cast.SetBoundingBox(position + Vector2((10.0f) * nx, 8.0f), Vector2(4.0f, 6.0f));

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
                        holdShell->AttachHold(this, -8.0f);
                    else
                        holdShell->AttachHold(this, 0.0f);
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

void CMario::marioPowerupUpdate(float dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - powerUpStartTimer > POWER_UP_ANIMATION_TIME)
    {
        powerUp = MARIO_POWERUP_BIG;
        SetState(MARIO_STATE_NORMAL);
        position.y -= 9.0f;
    }
}


void CMario::Update(float dt, vector<LPGAMEOBJECT>* coObjects) {

    if (kickTimer > 0) kickTimer -= dt;
    switch (state)
    {
    case MARIO_STATE_NORMAL: marioNormalUpdate(dt, coObjects); break;
    case MARIO_STATE_POWER_UP: marioPowerupUpdate(dt, coObjects); break;
    }
}

void CMario::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    LPANIMATION animation = animations->Get(GetAnimationID());
    switch (state)
    {
    case MARIO_STATE_NORMAL:
        {
            if (abs(velocity.x) <= MAXIMUM_WALK_SPEED + WALKING_ACCELERATION)
                animation->SetTimeScale(1.0f);
            else
                animation->SetTimeScale(0.5f);

            bool flipX = nx > 0 ? true : false;
            animation->Render(position.x, position.y, GetLayer(layer, orderInLayer), flipX);
        }
        break;
    case MARIO_STATE_POWER_UP:
        {
            bool flipX = nx > 0 ? true : false;
            if (animation->GetCurrentFrameIndex() == 1)
                animation->Render(position.x, position.y - 8.0f, GetLayer(layer, orderInLayer), flipX);
            else
                animation->Render(position.x, position.y, GetLayer(layer, orderInLayer), flipX);
        }
        break;
    }
    //RenderBoundingBox();
    //cast.RenderBoundingBox();
}

void CMario::SetState(int state) {
    switch (state)
    {
    case MARIO_STATE_NORMAL:
        {
            
        }
        break;
    case MARIO_STATE_POWER_UP:
        {
            if (this->state == MARIO_STATE_NORMAL)
            {
                powerUpStartTimer = GetTickCount64();
            }
        }
        break;
    }
    this->state = state;
}

void CMario::OnNoCollision(float dt) {
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
        questionBlock->Hit(sign(questionBlock->GetPosition().x - position.x));
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
            velocity.y = -ENEMY_BOUNCE;
        }
    }
    else if (dynamic_cast<CMushroom*>(e->obj))
    {
        e->obj->Delete();
        SetState(MARIO_STATE_POWER_UP);
        //powerUp = MARIO_POWERUP_BIG;
    }
    else if (dynamic_cast<CGreenKoopa*>(e->obj))
    {
        CGreenKoopa* const koopa = dynamic_cast<CGreenKoopa*>(e->obj);
        if (e->ny >= 0)
        {
            if (koopa->IsInShell() && koopa->GetVelocity().x == 0.0f)
            {
                if (e->ny == 0)
                    kickTimer = KICK_ANIMATION_TIME;
                CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(e->obj->GetPosition().x, e->obj->GetPosition().y, Score200));
                koopa->SetNx(nx);
            }
            cout << "ouch!!!\n";
        }
        else
        {
            int posSign = sign(position.x - koopa->GetPosition().x);
            CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(e->obj->GetPosition().x, e->obj->GetPosition().y, Score100));
            velocity.y = -ENEMY_BOUNCE;
            koopa->PlayerHit(posSign);
        }
    }
}

int CMario::GetAnimationIDSmall()
{
    if (state == MARIO_STATE_POWER_UP)
        return MARIO_ID_ANIMATION_LEVEL_UP;
    if (isHolding)
    {
        if (abs(velocity.x) > 0)
            return MARIO_ID_ANIMATION_HOLD_WALK;
        else
            return MARIO_ID_ANIMATION_HOLD_IDLE;
    }
    if (kickTimer > 0)
        return MARIO_ID_ANIMATION_SHELL_KICK;
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
    if (kickTimer > 0)
        return MARIO_BIG_ID_ANIMATION_SHELL_KICK;
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