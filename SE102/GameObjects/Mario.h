#pragma once

#include "Engine/GameObject.h"

#include "Engine/Math/Vector2.h"
#include "Engine/Game.h"
#include "Engine/CollisionCast.h"
#include "NPC/GreenKoopa.h"

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

constexpr float JUMP_INIT_VEL = 0x03800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float JUMP_HELD_GRAVITY = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float JUMP_GRAVITY = 0x00500 * SUBSUBSUBPIXEL_DELTA_TIME;

constexpr float JUMP_MAX_NEGATIVE = -2 * MAX_DELTA_TIME;

constexpr float MAX_FALL_SPEED = 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME;


constexpr float ENEMY_BOUNCE = 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME;
#pragma endregion

#pragma region Others

constexpr float POWER_UP_ANIMATION_TIME = 1500;
constexpr float KICK_ANIMATION_TIME = 300;

#pragma endregion

#define MARIO_POWERUP_SMALL 0
#define MARIO_POWERUP_BIG 1
#define MARIO_POWERUP_RACOON 2

#define KEY_MOVE_LEFT 'A'
#define KEY_MOVE_RIGHT 'D'
#define KEY_RUN 'J'
#define KEY_JUMP 'K'

#define MARIO_STATE_NORMAL 1
#define MARIO_STATE_POWER_UP 2

// Y ofset
// Held object Large: -$02
// Suit: $0D
// Small: $0F
// ObjectKickXVelMoving:	.byte -$30, $30
/*
	; Some objects have "feet" when they are waking up,
	; this offsets their Y depending on whether v-flipped
ObjWakeUp_FeetYOff:	.byte 10, -10
; In units of $10 ticks by timer 3...

ObjKickXvel:	.byte $18, -$18


	; Different X and X Hi offsets applied to object being held by Player
	; Changes whether not doing anything special, in pipe, etc.
ObjectHoldXOff:		.byte $0B, -$0B, $04, -$04, $04, $0B, -$13, $04, -$08, $04, $00
ObjectHoldXHiOff:	.byte $00,  $FF, $00,  $FF, $00, $00,  $FF, $00,  $FF, $00, $00

	; Object-to-object hit resultant X velocity
ObjectToObject_HitXVel:	.byte -$08, $08
EnemyKill_XVels:	.byte -$08, $08

Player grow 20
Player_Grow = $2f (shrinking!)

bounce #-$38
Larger bounce #-$70

	; Set Player's Y velocity to -$40 (bounce!)
	LDA #-$40

ENEMY
	; Set Y vel to -$20 (bounce up)

	; Set Y Vel to -$30 (bounce dead)
	LDA #-$30
*/

class CMario : public CGameObject {
private:
	int GetAnimationIDSmall();
	int GetAnimationIDBig();
	int GetAnimationID();
	Vector2 accel;
	float runBeforeWalkTimer = 0.0f;
	bool skidding = false;
	bool isOnGround = false;

	int powerUp = 0;
	float powerUpStartTimer = 0.0f;

	float kickTimer = 0.0f;

	CCollisionCast cast;
	CGreenKoopa* holdShell;
	bool isHolding = false;

	void marioNormalUpdate(float dt, vector<LPGAMEOBJECT>* coObjects);
	void marioPowerupUpdate(float dt, vector<LPGAMEOBJECT>* coObjects);

    static bool IsContentLoaded;
    static void LoadContent();
public:

	CMario(float x, float y);

    void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;

	int IsCollidable() override { return !isDeleted; };
	void OnNoCollision(float dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) override { return 1; }

	void SetState(int state) override;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		if (powerUp == MARIO_POWERUP_SMALL)
		{
			const Vector2 marioSize = Vector2(10.f, 16.f) / 2.0f;
			left = position.x - marioSize.x;
			top = position.y - marioSize.y;
			right = position. x + marioSize.x;
			bottom = position.y + marioSize.y - 1;
		}
		else
		{
			const float yOffset = 4.0f;
			const Vector2 marioSize = Vector2(12.f, 24.f) / 2.0f;
			left = position.x - marioSize.x;
			top = position.y - marioSize.y + yOffset;
			right = position.x + marioSize.x;
			bottom = position.y + marioSize.y + yOffset;

		}
	}
};