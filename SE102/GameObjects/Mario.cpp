#include "Mario.h"
#include "Engine/Loaders/SpritesLoader.h"

#include "Engine/Game.h"
#include "Engine/Graphics/Animations.h"

#include "ContentIds/Mario.h"
#include "ContentIds/MarioBig.h"
#include "ContentIds/MarioRacoon.h"

#include "Engine/Helper.h"
#include <Engine/debug.h>

#include "QuestionBlock.h"
#include "Coin.h"
#include "NPC/Goomba.h"
#include "NPC/GreenKoopa.h"
#include "Powerups/Mushroom.h"
#include "NPC/Fireball.h"
#include "NPC/FirePiranha.h"
#include "ScorePopup.h"

#include <iostream>
#include "Powerups/Leaf.h"
#include "Powerups/OneUp.h"
#include <Engine/PlayScene.h>
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
    loader.Load(MARIO_RACOON_SPRITES_PATH);
}

CMario::CMario(float x, float y) : CGameObject(x, y, 0.0f)
{
    LoadContent();
    nx = 1;
    holdCast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return dynamic_cast<CGreenKoopa*>(obj) != nullptr;
    });
    spinCast.SetConditionFunction([this](LPGAMEOBJECT obj) {
        return dynamic_cast<CGreenKoopa*>(obj) != nullptr ||
            dynamic_cast<CGoomba*>(obj) != nullptr || 
            dynamic_cast<CFirePiranha*>(obj) != nullptr;
    });
    layer = SortingLayer::MARIO;
    nextPowerUp = MARIO_POWERUP_RACOON;
    SetState(MARIO_STATE_POWER_UP);
}

void CMario::marioNormalUpdate(float dt, vector<LPGAMEOBJECT>* coObjects)
{

    CGame* const game = CGame::GetInstance();
    // Holding Shell
    isHolding = false;
    if (game->IsKeyDown(KEY_RUN))
    {
        if (powerUp == MARIO_POWERUP_SMALL)
            holdCast.SetBoundingBox(position + Vector2((10.0f) * nx, 0.0f), Vector2(4.0f, 6.0f));
        else
            holdCast.SetBoundingBox(position + Vector2((10.0f) * nx, 8.0f), Vector2(4.0f, 6.0f));

        holdCast.CheckOverlap(coObjects);
        if (holdCast.collision.size() > 0)
        {
            isHolding = true;
            if (holdShell == NULL)
            {
                holdShell = dynamic_cast<CGreenKoopa*>(holdCast.collision[0]);
                if (holdShell->IsInShell() && holdShell->GetVelocity().x == 0.0f)
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

    if (spinTimer > 0 && powerUp == MARIO_POWERUP_RACOON)
    {
        LPANIMATION animation = CAnimations::GetInstance()->Get(MARIO_RACOON_ID_ANIMATION_SPIN);

        spinCast.SetBoundingBox(position + Vector2(0.0f, 8.0f), Vector2(40.0f, 15.0f));
        //spinCast.CheckOverlap(coObjects);
        if (animation->GetCurrentFrameIndex() == 0 || animation->GetCurrentFrameIndex() == 4)
        {
            //spinCast.SetBoundingBox(position + Vector2(10.0f * nx, 8.0f), Vector2(25.0f, 15.0f));
            spinCast.CheckOverlap(coObjects);
        }
        if (animation->GetCurrentFrameIndex() == 2)
        {
            //spinCast.SetBoundingBox(position + Vector2(10.0f * -nx, 8.0f), Vector2(25.0f, 15.0f));
            spinCast.CheckOverlap(coObjects);
        }

        if (spinCast.collision.size() > 0)
        {
            for (LPGAMEOBJECT obj : spinCast.collision)
            {
                if (obj == NULL || obj->IsDeleted()) continue;
                if (dynamic_cast<CGoomba*>(obj) != NULL)
                {
                    CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
                    goomba->DeadBounce();
                }
                else if (dynamic_cast<CGreenKoopa*>(obj) != NULL)
                {
                    CGreenKoopa* koopa = dynamic_cast<CGreenKoopa*>(obj);
                    koopa->DeadBounce();
                }
                else if (dynamic_cast<CFirePiranha*>(obj) != NULL)
                {
                    CFirePiranha* piranha = dynamic_cast<CFirePiranha*>(obj);
                    piranha->Delete();
                }
            }
        }
    }

    if (game->IsKeyJustPressed(KEY_RUN) && powerUp == MARIO_POWERUP_RACOON)
    {
        LPANIMATION animation = CAnimations::GetInstance()->Get(MARIO_RACOON_ID_ANIMATION_SPIN);
        animation->Reset();
        animation->SetLoop(false);
        spinTimer = SPIN_TIME;
    }

    if (!isHolding && holdShell != NULL)
    {
        holdShell->DetachHold();
        holdShell = NULL;
    }

    // X Movement
    accel.x = 0;

    bool keyRunDown = false;
    if (game->IsKeyDown(KEY_RUN))
        keyRunDown = true;
    if (game->IsKeyJustReleased(KEY_RUN)) {
        runBeforeWalkTimer = RUN_TIME_BEFORE_WALK;
    }

    if (runBeforeWalkTimer > 0)
        runBeforeWalkTimer -= dt;

    if (game->IsKeyDown(KEY_MOVE_LEFT)) {
        accel.x = -(keyRunDown ? RUNNING_ACCELERATION : WALKING_ACCELERATION);
        nx = -1;
    }
    if (game->IsKeyDown(KEY_MOVE_RIGHT)) {
        accel.x = (keyRunDown ? RUNNING_ACCELERATION : WALKING_ACCELERATION);
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
        if (isOnGround)
            skidding = true;
        velocity.x = move_towards(velocity.x, 0, SKIDDING_DECELERATION);
    }
    if (powerCounter >= MAX_POWER_COUNT)
        velocity.x = clampf(velocity.x, -MAXIMUM_POWER_SPEED, MAXIMUM_POWER_SPEED);
    else if (keyRunDown || (abs(velocity.x) > MAXIMUM_WALK_SPEED + WALKING_ACCELERATION && runBeforeWalkTimer > 0))
        velocity.x = clampf(velocity.x, -MAXIMUM_RUNNING_SPEED, MAXIMUM_RUNNING_SPEED);
    else
        velocity.x = clampf(velocity.x, -MAXIMUM_WALK_SPEED, MAXIMUM_WALK_SPEED);

    // Dashing
    if (keyRunDown)
    {

        if (abs(velocity.x) >= MAXIMUM_RUNNING_SPEED)
        {
            if (powerTimer > 0) powerTimer -= dt;
            else
            {
                powerTimer = POWER_TIME;
                powerCounter = clampi(powerCounter + 1, 0, MAX_POWER_COUNT);
            }
            powerReduceTimer = POWER_REDUCE_TIME;
        }
        else if (powerCounter >= MAX_POWER_COUNT)
            powerCounter--;
    }

    if (powerUp == MARIO_POWERUP_RACOON && powerCounter >= MAX_POWER_COUNT && isOnGround)
        flightTimer = FLY_P_TIMER;


    if (powerReduceTimer > 0) powerReduceTimer -= dt;
    else if (powerCounter > 0)
    {
        powerReduceTimer = POWER_REDUCE_TIME;
        powerCounter = clampi(powerCounter - 1, 0, MAX_POWER_COUNT);
    }



    // Y MOVEMENT
    float gravity = 0.0f;
    if ((game->IsKeyDown(KEY_JUMP) && velocity.y <= JUMP_MAX_NEGATIVE))
        gravity = JUMP_HELD_GRAVITY;
    else
        gravity = JUMP_GRAVITY;
    accel.y = gravity;

    // Flying
    if (flightTimer > 0) flightTimer -= dt;
    if (flying)
    {
        // Disable flying
        if (flightTimer <= 0.0f)
        {
            powerCounter = 0;
        }
        if (isOnGround)
        {
            powerCounter = 0;
            flying = false;
        }
    }

    if (superJump && isOnGround)
    {
        superJump = false;
        //powerCounter = 0;
    }

    // Tail things
    if (velocity.y >= FLY_Y_VELOCITY && flightTimer > 0)
    {
        if (game->IsKeyDown(KEY_JUMP) && powerUp == MARIO_POWERUP_RACOON)
            accel.y = FLY_Y_VELOCITY;
        // Does this really matter?
        //constexpr float FLY_SMALL_TIME = 0x0F * 1000 / 60;
        //constexpr int FLY_APEX = 0x08 * 1000 / 60;
        //if (flightTimer > 0 && flightTimer < FLY_SMALL_TIME && (int)(flightTimer) % FLY_APEX == 0)
        //{
        //    accel.y = 0;
        //}
    }

    if (wagTimer > 0) wagTimer -= dt;
    if (game->IsKeyJustPressed(KEY_JUMP) && powerCounter < MAX_POWER_COUNT && powerUp == MARIO_POWERUP_RACOON)
        wagTimer = WAG_TIME;

    // JUmp
    if (game->IsKeyJustPressed(KEY_JUMP) && isOnGround) {
        float initVel = -JUMP_INIT_VEL;
        float absVelX = abs(velocity.x);
        if (absVelX <= 0.0f)
            initVel -= 0x00000 * SUBSUBSUBPIXEL_DELTA_TIME;
        else if (absVelX <= MAXIMUM_WALK_SPEED)
            initVel -= 0x00200 * SUBSUBSUBPIXEL_DELTA_TIME;
        else if (absVelX <= MAXIMUM_RUNNING_SPEED)
            initVel -= 0x00400 * SUBSUBSUBPIXEL_DELTA_TIME;
        else if (absVelX <= MAXIMUM_POWER_SPEED)
            initVel -= 0x00800 * SUBSUBSUBPIXEL_DELTA_TIME;

        wagTimer = 0.0f;
        if (flightTimer > 0)
            flying = true;
        if (powerCounter >= MAX_POWER_COUNT)
            superJump = true;

        accel.y = initVel;
    }

    if (game->IsKeyDown(KEY_DOWN) && powerUp != MARIO_POWERUP_SMALL && isOnGround)
        SetState(MARIO_STATE_SIT);

    velocity.x += accel.x;
    velocity.y += accel.y;

    if (wagTimer > 0.0f)
        velocity.y = min(velocity.y, MAX_TAILWAG_FALL_SPDED);
    else
        velocity.y = min(velocity.y, MAX_FALL_SPEED);

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::marioPowerupUpdate(float dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - powerUpStartTimer > POWER_UP_ANIMATION_TIME)
    {
        CGame::GetInstance()->SetTimeScale(1.0f);
        powerUp = nextPowerUp;
        SetState(MARIO_STATE_NORMAL);
        if (nextPowerUp != MARIO_POWERUP_SMALL)
            position.y -= 9.0f;
        else
            invincibleTimer = INVINCIBLE_TIME;
    }
}


void CMario::Update(float dt, vector<LPGAMEOBJECT>* coObjects) {

    if (kickTimer > 0) kickTimer -= dt;
    if (invincibleTimer > 0) invincibleTimer -= dt;
    if (spinTimer > 0) spinTimer -= dt;
    switch (state)
    {
    case MARIO_STATE_NORMAL: marioNormalUpdate(dt, coObjects); break;
    case MARIO_STATE_POWER_UP: marioPowerupUpdate(dt, coObjects); break;
    case MARIO_STATE_DEAD: 
    {
        CGame* const game = CGame::GetInstance();
        ULONGLONG unscaledDt = game->GetUnscaledDt();
        if (deadTimer > 0) deadTimer -= unscaledDt;
        else
        {
            if (!deadJump)
            {
                velocity.y = -0.8f;
                deadJump = true;
            }
            //velocity.y = min(velocity.y + JUMP_GRAVITY / 2.0f, MAX_FALL_SPEED / 2.0f);
            velocity.y = velocity.y + JUMP_GRAVITY / 2.0f;
            position.y += velocity.y / 2.0f * unscaledDt;
        }

        if (!IsColliderInCamera())
        {
            if (!isResetting)
            {
                isResetting = true;
                levelResetTimer = DEAD_RESET_TIME;
            }

            if (levelResetTimer > 0) levelResetTimer -= unscaledDt;
            else
            {
                CGame::GetInstance()->SetResetScene(true);
            }
        }
    }
    break;
    case MARIO_STATE_SIT:
    {
        if (!CGame::GetInstance()->IsKeyDown(KEY_DOWN))
            SetState(MARIO_STATE_NORMAL);
        velocity.x = move_towards(velocity.x, 0, RELEASE_DECELERATION);
        velocity.y = min(velocity.y + JUMP_GRAVITY, MAX_FALL_SPEED);
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }
    break;
    }
}

void CMario::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    if (invincibleTimer > 0 && (int)(invincibleTimer) % 200 <= 100)
        return;


    LPANIMATION animation = animations->Get(GetAnimationID());
    if (state == MARIO_STATE_POWER_UP)
    {
        if (nextPowerUp == MARIO_POWERUP_RACOON)
            return;
        bool flipX = nx > 0 ? true : false;
        float yOffset = 0.0f;
        if (animation->GetCurrentFrameIndex() >= 1 && nextPowerUp != MARIO_POWERUP_SMALL)
            yOffset = -8.0f;
        if (animation->GetCurrentFrameIndex() == 0 && nextPowerUp == MARIO_POWERUP_SMALL)
            yOffset = -8.0f;
        if (nextPowerUp < powerUp && powerUp == MARIO_POWERUP_RACOON)
            yOffset = -8.0f;
        animation->Render(position.x, position.y + yOffset, GetLayer(layer, orderInLayer), flipX);
    }
    else
    {
        if (abs(velocity.x) <= MAXIMUM_WALK_SPEED + WALKING_ACCELERATION)
            animation->SetTimeScale(1.0f);
        else
            animation->SetTimeScale(0.5f);

        if (spinTimer > 0.0f)
            animation->SetTimeScale(1.0f);

        bool flipX = nx > 0 ? true : false;
        animation->Render(position.x, position.y, GetLayer(layer, orderInLayer), flipX);

    }
    //RenderBoundingBox();
    spinCast.RenderBoundingBox();
}

void CMario::SetState(int state) {
    switch (state)
    {
    case MARIO_STATE_NORMAL:
    {
        if (this->state == MARIO_STATE_SIT)
            position.y -= 0.0f;
    }
    break;
    case MARIO_STATE_POWER_UP:
    {
        if (this->state == MARIO_STATE_NORMAL)
        {
            CGame::GetInstance()->SetTimeScale(0.0f);
            powerUpStartTimer = (float)GetTickCount64();
        }
    }
    break;
    case MARIO_STATE_DEAD:
    {
        if (DEBUG_INVINCIBLE)
            return;
        if (invincibleTimer > 0)
            return;
        if (powerUp != MARIO_POWERUP_SMALL)
        {
            state = MARIO_STATE_POWER_UP;
            CGame::GetInstance()->SetTimeScale(0.0f);
            if (powerUp == MARIO_POWERUP_RACOON)
                nextPowerUp = MARIO_POWERUP_BIG;
            else    
                nextPowerUp = MARIO_POWERUP_SMALL;
            position.y += 8.0f;
            powerUpStartTimer = (float)GetTickCount64();
        }
        else 
        {
            deadTimer = DEAD_STAY_TIME;
            velocity = Vector2::Zero;
            CGame::GetInstance()->SetTimeScale(0.0f);
            layer = SortingLayer::CORPSE;
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
        if (questionBlock->GetSpawnType() == QUESTION_BLOCK_SPAWN_COIN && questionBlock->GetSpawnCount() > 0)
        {
            coinCounter++;
        }
        questionBlock->Hit(sign(questionBlock->GetPosition().x - position.x));
    }
    else if (dynamic_cast<CCoin*>(e->obj) && e->obj->GetState() == COIN_STATE_NORMAL)
    {
        coinCounter++;
        e->obj->Delete();
    }
    else if (dynamic_cast<CGoomba*>(e->obj))
    {
        CGoomba* const goomba = dynamic_cast<CGoomba*>(e->obj);
        if (goomba->GetState() != GOOMBA_STATE_DEAD && goomba->GetState() != GOOMBA_STATE_DEAD_BOUNCE)
        {
            if (e->ny >= 0)
            {
                SetState(MARIO_STATE_DEAD);
            }
            else
            {
                goomba->SetKill();
                velocity.y = -ENEMY_BOUNCE;
            }
        }
    }
    else if (dynamic_cast<COneUp*>(e->obj))
    {
        dynamic_cast<COneUp*>(e->obj)->Eat();

    }
    else if (dynamic_cast<CMushroom*>(e->obj))
    {
        e->obj->Delete();
        nextPowerUp = MARIO_POWERUP_BIG;
        if (powerUp != nextPowerUp)
            SetState(MARIO_STATE_POWER_UP);
        else
        {
            CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(e->obj->GetPosition().x, e->obj->GetPosition().y, Score1000));
        }
    }
    else if (dynamic_cast<CLeaf*>(e->obj))
    {
        e->obj->Delete();
        nextPowerUp = MARIO_POWERUP_RACOON;
        if (powerUp != nextPowerUp)
            SetState(MARIO_STATE_POWER_UP);
        else
        {
            CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(e->obj->GetPosition().x, e->obj->GetPosition().y, Score1000));
        }
       
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
            else
            {
                SetState(MARIO_STATE_DEAD);
            }
        }
        else
        {

            int posSign = sign(position.x - koopa->GetPosition().x);
            CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(e->obj->GetPosition().x, e->obj->GetPosition().y, Score100));
            velocity.y = -ENEMY_BOUNCE;
            koopa->PlayerHit(posSign);
        }
    }
    else if (dynamic_cast<CFireball*>(e->obj))
    {
        SetState(MARIO_STATE_DEAD);
    }
    else if (dynamic_cast<CFirePiranha*>(e->obj))
    {
        SetState(MARIO_STATE_DEAD);
    }
}

int CMario::GetAnimationIDSmall()
{
    if (state == MARIO_STATE_DEAD)
        return MARIO_ID_ANIMATION_DEAD;
    if (state == MARIO_STATE_POWER_UP)
        return MARIO_ID_ANIMATION_LEVEL_UP;
    if (isHolding)
    {
        if (abs(velocity.x) > 0)
            return MARIO_ID_ANIMATION_HOLD_WALK;
        else
            return MARIO_ID_ANIMATION_HOLD_IDLE;
    }
    if (powerCounter >= MAX_POWER_COUNT && !isOnGround)
        return MARIO_ID_ANIMATION_FLY;
    if (skidding)
        return MARIO_ID_ANIMATION_SKIDDING;
    if (powerCounter >= MAX_POWER_COUNT)
        return MARIO_ID_ANIMATION_RUNFLY;
    if (kickTimer > 0)
        return MARIO_ID_ANIMATION_SHELL_KICK;
    if (!isOnGround)
        return MARIO_ID_ANIMATION_JUMP;
    if (abs(velocity.x) > 0)
        return MARIO_ID_ANIMATION_WALK;
    else
        return MARIO_ID_ANIMATION_IDLE;
}

int CMario::GetAnimationIDBig()
{
    if (state == MARIO_STATE_SIT)
        return MARIO_BIG_ID_ANIMATION_SIT;
    if (state == MARIO_STATE_POWER_UP)
        return MARIO_BIG_ID_ANIMATION_DEAD;
    if (isHolding)
    {
        if (abs(velocity.x) > 0)
            return MARIO_BIG_ID_ANIMATION_HOLD_WALK;
        else
            return MARIO_BIG_ID_ANIMATION_HOLD_IDLE;
    }
    if (powerCounter >= MAX_POWER_COUNT && !isOnGround)
        return MARIO_BIG_ID_ANIMATION_FLY;
    if (skidding)
        return MARIO_BIG_ID_ANIMATION_SKIDDING;
    if (powerCounter >= MAX_POWER_COUNT)
        return MARIO_BIG_ID_ANIMATION_RUNFLY;
    if (kickTimer > 0)
        return MARIO_BIG_ID_ANIMATION_SHELL_KICK;
    if (!isOnGround)
        return MARIO_BIG_ID_ANIMATION_JUMP;
    if (abs(velocity.x) > 0)
        return MARIO_BIG_ID_ANIMATION_WALK;
    else
        return MARIO_BIG_ID_ANIMATION_IDLE;
}


int CMario::GetAnimationIDRacoon()
{
    if (state == MARIO_STATE_SIT)
        return MARIO_RACOON_ID_ANIMATION_SIT;
    if (state == MARIO_STATE_POWER_UP)
        return MARIO_RACOON_ID_ANIMATION_DEAD;
    if (isHolding)
    {
        if (abs(velocity.x) > 0)
            return MARIO_RACOON_ID_ANIMATION_HOLD_WALK;
        else
            return MARIO_RACOON_ID_ANIMATION_HOLD_IDLE;
    }
    if (spinTimer > 0.0f)
        return MARIO_RACOON_ID_ANIMATION_SPIN;
    if (powerCounter >= MAX_POWER_COUNT && !isOnGround)
        return MARIO_RACOON_ID_ANIMATION_FLY;
    if (skidding)
        return MARIO_RACOON_ID_ANIMATION_SKIDDING;
    if (powerCounter >= MAX_POWER_COUNT)
        return MARIO_RACOON_ID_ANIMATION_RUNFLY;
    if (kickTimer > 0)
        return MARIO_RACOON_ID_ANIMATION_SHELL_KICK;
    if (!isOnGround)
    {
        if (wagTimer > 0)
            return MARIO_RACOON_ID_ANIMATION_AIR_TAILWAG;
        if (velocity.y < 0.0f)
            return MARIO_RACOON_ID_ANIMATION_JUMP;
        return MARIO_RACOON_ID_ANIMATION_FALL;
    }
    if (abs(velocity.x) > 0)
        return MARIO_RACOON_ID_ANIMATION_WALK;
    else
        return MARIO_RACOON_ID_ANIMATION_IDLE;
}

int CMario::GetAnimationID()
{
    switch (powerUp)
    {
    case MARIO_POWERUP_SMALL: return GetAnimationIDSmall();
    case MARIO_POWERUP_BIG: return GetAnimationIDBig();
    case MARIO_POWERUP_RACOON: return GetAnimationIDRacoon();
    }
    return 0;
}