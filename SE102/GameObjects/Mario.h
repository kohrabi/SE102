#pragma once

#include "Engine/GameObject.h"

#include "Engine/Math/Vector2.h"
#include "Engine/Game.h"

#define SUBPIXEL 1.0F/16.0F
#define MAX_DELTA_TIME 65.f / 1000.0f
#define SUBSUBSUBPIXEL SUBPIXEL * SUBPIXEL * SUBPIXEL
#define SUBSUBSUBPIXEL_DELTA_TIME SUBSUBSUBPIXEL * MAX_DELTA_TIME

#pragma region X MOVEMENT
#define MINIMUM_WALK_VELOCITY 0x00098 * SUBSUBSUBPIXEL_DELTA_TIME
#define WALKING_ACCELERATION 0x00098 * SUBSUBSUBPIXEL_DELTA_TIME
#define RUNNING_ACCELERATION 0x000E4 * SUBSUBSUBPIXEL_DELTA_TIME

#define MAXIMUM_WALK_SPEED 0x01900 * SUBSUBSUBPIXEL_DELTA_TIME
#define MAXIMUM_RUNNING_SPEED 0x02900 * SUBSUBSUBPIXEL_DELTA_TIME

#define RELEASE_DECELERATION 0x000D0 * SUBSUBSUBPIXEL_DELTA_TIME
#define SKIDDING_DECELERATION 0x001A0 * SUBSUBSUBPIXEL_DELTA_TIME

#define RUN_TIME_BEFORE_WALK 10.0f * MAX_DELTA_TIME

#pragma endregion
#pragma region Y MOVEMENT

#define JUMP_SLOW_INIT_VEL 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME
#define JUMP_SLOW_HELD_GRAVITY 0x00200 * SUBSUBSUBPIXEL_DELTA_TIME
#define JUMP_SLOW_GRAVITY 0x00700 * SUBSUBSUBPIXEL_DELTA_TIME

#define SLOW_VEL 0x01000 * SUBSUBSUBPIXEL

#define JUMP_MID_INIT_VEL 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME
#define JUMP_MID_HELD_GRAVITY 0x001E0 * SUBSUBSUBPIXEL_DELTA_TIME
#define JUMP_MID_GRAVITY 0x00600 * SUBSUBSUBPIXEL_DELTA_TIME

#define MID_VEL_START 0x01000 * SUBSUBSUBPIXEL
#define MID_VEL_END 0x024FF * SUBSUBSUBPIXEL

#define JUMP_FAST_INIT_VEL 0x05000 * SUBSUBSUBPIXEL_DELTA_TIME
#define JUMP_FAST_HELD_GRAVITY 0x00280 * SUBSUBSUBPIXEL_DELTA_TIME
#define JUMP_FAST_GRAVITY 0x00900 * SUBSUBSUBPIXEL_DELTA_TIME

#define FAST_VEL 0x02500 * SUBSUBSUBPIXEL

#define MAX_FALL_SPEED 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME

#pragma endregion

#define KEY_MOVE_LEFT 'A'
#define KEY_MOVE_RIGHT 'D'
#define KEY_RUN 'J'
#define KEY_JUMP 'K'

class CMario : public CGameObject {
private:
	int GetAnimationIDSmall();
	Vector2 accel;
	float runBeforeWalkTimer = 0.0f;
	bool skidding = false;

	bool isOnGround = false;
    static bool IsContentLoaded;
    static void LoadContent();
public:

    CMario(float x, float y) : CGameObject(x, y, 0.0f) 
	{
		LoadContent();
	}

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;

	int IsCollidable() override { return !isDeleted; };
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) override { return 1; }

	void SetState(int state) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
        const Vector2 MarioSize = Vector2(16.f, 16.f) / 2.0f;
		left = position.x - MarioSize.x;
		top = position.y - MarioSize.y;
		right = position. x + MarioSize.x;
		bottom = position.y + MarioSize.y - 1;
	}
};