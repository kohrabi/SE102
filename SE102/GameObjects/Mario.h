#pragma once

#include "Engine/GameObject.h"

#include "Engine/Math/Vector2.h"
#include "Engine/Game.h"

#pragma region X MOVEMENT
constexpr float MINIMUM_WALK_VELOCITY = 0x00098 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float WALKING_ACCELERATION = 0x00098 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float RUNNING_ACCELERATION = 0x000E4 * SUBSUBSUBPIXEL_DELTA_TIME;
		  
constexpr float MAXIMUM_WALK_SPEED = 0x01800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float MAXIMUM_RUNNING_SPEED = 0x02800 * SUBSUBSUBPIXEL_DELTA_TIME;
		  
constexpr float RELEASE_DECELERATION = 0x000D0 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float SKIDDING_DECELERATION = 0x001A0 * SUBSUBSUBPIXEL_DELTA_TIME;
		  
constexpr float RUN_TIME_BEFORE_WALK = 10.0f * MAX_DELTA_TIME;

#pragma endregion
#pragma region Y MOVEMENT

constexpr float JUMP_SLOW_INIT_VEL = 0x03800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float JUMP_SLOW_HELD_GRAVITY = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float JUMP_SLOW_GRAVITY = 0x00500 * SUBSUBSUBPIXEL_DELTA_TIME;

constexpr float JUMP_MAX_NEGATIVE = -2 * MAX_DELTA_TIME;

constexpr float MAX_FALL_SPEED = 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME;

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
        const Vector2 MarioSize = Vector2(10.f, 16.f) / 2.0f;
		left = position.x - MarioSize.x;
		top = position.y - MarioSize.y;
		right = position. x + MarioSize.x;
		bottom = position.y + MarioSize.y - 1;
	}
};