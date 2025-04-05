#pragma once

#include "Engine/GameObject.h"

#include "Engine/Math/Vector2.h"
#include "Engine/Game.h"

#define SUBPIXEL 1.0F/16.0F
#define MAX_DELTA_TIME 1.0f / MAX_FRAME_RATE
#define SUBSUBSUBPIXEL SUBPIXEL * SUBPIXEL * SUBPIXEL
// IN SUB.SUB.SUBPIXEL
#define MINIMUM_WALK_VELOCITY 0x00098 * SUBSUBSUBPIXEL
#define WALKING_ACCELERATION 0x00098 * SUBSUBSUBPIXEL
#define RUNNING_ACCELERATION 0x000E4 * SUBSUBSUBPIXEL

#define MAXIMUM_WALK_SPEED 0x01900 * SUBSUBSUBPIXEL
#define MAXIMUM_RUNNING_SPEED 0x02900 * SUBSUBSUBPIXEL

#define RELEASE_DECELERATION 0x000D0 * SUBSUBSUBPIXEL
#define SKIDDING_DECELERATION 0x001A0 * SUBSUBSUBPIXEL

#define KEY_MOVE_LEFT 'A'
#define KEY_MOVE_RIGHT 'D'
#define KEY_RUN 'J'

class CMario : public CGameObject {
private:
	int GetAnimationIDSmall();
	Vector2 accel;
public:
    static bool IsContentLoaded;
    static void LoadContent();

    CMario(float x, float y) : CGameObject(x, y, 0.0f) 
	{
		LoadContent();
	}

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;

	int IsCollidable() override { return !isDeleted; };
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsBlocking() override { return 1; }
	int IsDirectionColliable(float nx, float ny) override { return 1; }

	void SetState(int state) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
        const Vector2 MarioSize = Vector2(16.f, 16.f) / 2.0f;
		left = position.x - MarioSize.x;
		top = position.y - MarioSize.y;
		right = position. x + MarioSize.x;
		bottom = position.y + MarioSize.y;
	}
};