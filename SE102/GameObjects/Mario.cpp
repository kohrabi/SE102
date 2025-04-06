#include "Mario.h"
#include "Engine/Loaders/SpritesLoader.h"

#include "Engine/Game.h"
#include "Engine/Graphics/Animations.h"

#include "contents.h"

#include "Engine/Helper.h"
#include <Engine/debug.h>

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
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    CGame* const game = CGame::GetInstance();
    float dts = dt / 1000.f;
    
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
    if (game->IsKeyDown(KEY_JUMP))
        accel.y = HOLDING_A_GRAVITY;
    else 
        accel.y = GRAVITY;
    
    if (game->IsKeyJustPressed(KEY_JUMP) && isOnGround) {
        accel.y = -INIT_JUMP_VEL;
    }

    velocity.x += accel.x;
    velocity.y += accel.y;

    velocity.y = min(velocity.y, MAX_FALL_SPEED);

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    LPANIMATION animation = animations->Get(GetAnimationIDSmall());
    if (abs(velocity.x) <= MAXIMUM_WALK_SPEED + WALKING_ACCELERATION)
        animation->SetTimeScale(1.0f);
    else
        animation->SetTimeScale(0.5f);

    bool flipX = nx > 0 ? true : false;
    animation->Render(position.x, position.y, flipX);
    // RenderBoundingBox();
}

int CMario::GetAnimationIDSmall()
{
    if (!isOnGround)
        return MARIO_ID_ANIMATION_JUMP;
    if (skidding)
        return MARIO_ID_ANIMATION_SKIDDING;
    if (abs(velocity.x) > 0)
        return MARIO_ID_ANIMATION_WALKING;
    else
        return MARIO_ID_ANIMATION_IDLE;
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
}