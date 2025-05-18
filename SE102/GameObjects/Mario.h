#pragma once

#include "Engine/GameObject.h"

#include "Engine/Math/Vector2.h"
#include "Engine/Game.h"
#include "Engine/CollisionCast.h"
#include "NPC/Koopa.h"

#define DEBUG_INVINCIBLE false

#pragma region X MOVEMENT
constexpr float MINIMUM_WALK_VELOCITY = 0x00098 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float WALKING_ACCELERATION = 0x00098 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float RUNNING_ACCELERATION = 0x000E4 * SUBSUBSUBPIXEL_DELTA_TIME;
		  
constexpr float MAXIMUM_WALK_SPEED = 0x01800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float MAXIMUM_RUNNING_SPEED = 0x02800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float MAXIMUM_POWER_SPEED = 0x03800 * SUBSUBSUBPIXEL_DELTA_TIME;
		  
constexpr float RELEASE_DECELERATION = 0x000D0 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float SKIDDING_DECELERATION = 0x001A0 * SUBSUBSUBPIXEL_DELTA_TIME;
		  
constexpr float RUN_TIME_BEFORE_WALK = 10.0f * MAX_DELTA_TIME;

#pragma endregion
#pragma region Y MOVEMENT

constexpr float JUMP_INIT_VEL = 0x03800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float JUMP_HELD_GRAVITY = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float JUMP_GRAVITY = 0x00500 * SUBSUBSUBPIXEL_DELTA_TIME;

constexpr float JUMP_MAX_NEGATIVE = -0x02000 * SUBSUBSUBPIXEL_DELTA_TIME;

constexpr float MAX_FALL_SPEED = 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float MAX_TAILWAG_FALL_SPDED = 0x01000 * SUBSUBSUBPIXEL_DELTA_TIME;

constexpr float FLY_Y_VELOCITY = -0x01800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float TELEPORT_Y_VELOCITY = 0x00B00 * SUBSUBSUBPIXEL_DELTA_TIME;

constexpr float ENEMY_BOUNCE = 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME;
#pragma endregion

#pragma region Others

constexpr float POWER_TIME = 8 * 1000.0f / 60.0f * 1.0f;
constexpr float POWER_REDUCE_TIME = 23 * 1000.0f / 60.0f;
constexpr int MAX_POWER_COUNT = 7;
constexpr float FLY_P_TIMER = 0x80 * 1000.0f / 60.0f;

constexpr float POWER_UP_ANIMATION_TIME = 700.0f;
constexpr float KICK_ANIMATION_TIME = 300.0f;

constexpr float DEAD_STAY_TIME = 1000.0f;
constexpr float DEAD_RESET_TIME = 1000.0f;

constexpr float INVINCIBLE_TIME = 1000.0f;
constexpr float WAG_TIME = 0x10 * 1000.0f / 60.0f;

constexpr float SPIN_TIME = 6 * 50.0f;

constexpr float STAY_OUTRO_TIME = 1000.0f;
constexpr float SWITCH_LEVEL_TIME = 3000.0f;

constexpr float COMBO_TIME = 1000.0f;

#pragma endregion

#define MARIO_POWERUP_SMALL 0
#define MARIO_POWERUP_BIG 1
#define MARIO_POWERUP_RACOON 2

#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_DOWN VK_DOWN
#define KEY_UP VK_UP
#define KEY_RUN 'A'
#define KEY_JUMP 'S'

#define MARIO_STATE_NORMAL 1
#define MARIO_STATE_POWER_UP 2
#define MARIO_STATE_SIT 3
#define MARIO_STATE_TELEPORT 4
#define MARIO_STATE_OUTRO 5
#define MARIO_STATE_DEAD 6

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
	int GetAnimationIDRacoon();
	int GetAnimationID();
	Vector2 accel;
	float runBeforeWalkTimer = 0.0f;
	bool skidding = false;
	bool isOnGround = false;

	int powerUp = 0;
	int nextPowerUp = 0;
	float powerUpStartTimer = 0.0f;

	float powerTimer = 0.0f;
	int powerCounter = 0;
	float powerReduceTimer = 0.0f;
	float wagTimer = 0.0f;

	bool flying = false;
	bool superJump = false;
	float flightTimer = 0.0f;

	float invincibleTimer = 0.0f;

	float kickTimer = 0.0f;

	float spinTimer = 0.0f;

	CCollisionCast turnCast;
	CCollisionCast spinCast;
	CKoopa* holdShell;
	bool isHolding = false;

	float deadTimer = 0.0f;
	bool deadJump = false;

	bool isResetting = false;
	float levelResetTimer = 0.0f;

	int coinCounter = 0;

	int comboCounter = 0;
	float comboTimer = 0.0f;

	float outroStayTimer = 0.0f;
	float switchLevelTimer = 0.0f;

	float beforeTeleportY = 0.0f;
	bool enterPipe = false;
	Vector2 teleportPosition = Vector2::Zero;

	float hitParticleTimer = 0.0f;
	Vector2 hitParticlePosition = Vector2::Zero;

	void marioNormalUpdate(float dt, vector<LPGAMEOBJECT>* coObjects);
	void marioPowerupUpdate(float dt, vector<LPGAMEOBJECT>* coObjects);

    static bool IsContentLoaded;
    static void LoadContent();
public:

	CMario(float x, float y);

    void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;

	int GetComboCount() const { return comboCounter; }
	void ResetComboTimer() { comboTimer = COMBO_TIME; comboCounter++; }
	int GetCoinCount() const { return coinCounter; }
	int GetPowerCount() const { return powerCounter; }
	bool IsOnGround() const { return isOnGround; }

	int IsCollidable() override { return !isDeleted && (state != MARIO_STATE_DEAD || state != MARIO_STATE_TELEPORT); };
	void OnNoCollision(float dt) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) override { return 1; }
	int GetPowerUp() const { return powerUp; }

	void SetState(int state) override;
	void Kill() { powerUp = MARIO_POWERUP_SMALL; SetState(MARIO_STATE_DEAD); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
};