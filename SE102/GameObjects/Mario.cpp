#include "Mario.h"
#include "Engine/Loaders/SpritesLoader.h"

#include "Engine/Game.h"
#include "Engine/Graphics/Animations.h"

#include "contents.h"

#include "Engine/helper.h"
#include <Engine/debug.h>

bool CMario::IsContentLoaded = false;

void CMario::LoadContent() {
    if (IsContentLoaded)
        return;
    CMario::IsContentLoaded = true;

    SpritesLoader loader;
    loader.Load(L"Content/Sprites/mario.txt");
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    CGame* const game = CGame::GetInstance();
    accel.x = 0;

    bool run = false;
    if (game->IsKeyDown(KEY_RUN))
        run = true;
   
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

    if (accel.x == 0.0f) {
        velocity.x = move_towards(velocity.x, 0, RELEASE_DECELERATION);
    }
    else if (sign(accel.x) != sign(velocity.x)) {
        velocity.x = move_towards(velocity.x, 0, SKIDDING_DECELERATION);
    }

    if (!run || abs(velocity.x) > MAXIMUM_WALK_SPEED)
        velocity.x = clampf(velocity.x, -MAXIMUM_WALK_SPEED, MAXIMUM_WALK_SPEED);
    else
        velocity.x = clampf(velocity.x, -MAXIMUM_RUNNING_SPEED, MAXIMUM_RUNNING_SPEED);
    DebugOut(L"%f\n", velocity.x);

    velocity.x += accel.x;
    velocity.y += accel.y;

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::Render() {
    CAnimations* const animations = CAnimations::GetInstance();

    LPANIMATION animation = animations->Get(GetAnimationIDSmall());
    bool flipX = nx > 0 ? true : false;
    animation->Render(position.x, position.y, flipX);
}

int CMario::GetAnimationIDSmall()
{
    if (abs(velocity.x) > 0)
        return MARIO_ID_ANIMATION_WALKING;
    else
        return MARIO_ID_ANIMATION_IDLE;
}

void CMario::SetState(int state) {
    this->state = state;
}

void CMario::OnNoCollision(DWORD dt) {
	position.x += velocity.x;
	position.y += velocity.y;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e) {

}