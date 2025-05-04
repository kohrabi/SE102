#pragma once


#include "Engine/GameObject.h"
#include "Engine/CollisionCast.h"

#include "Engine/Graphics/Animations.h"

#define GREEN_KOOPA_INTRO_Y_VELOCITY 0.03

constexpr float GREEN_KOOPA_X_SPEED = 0x00800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float GREEN_KOOPA_SHELL_X_SPEED = 0x02700 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float GREEN_KOOPA_KILL_TIME = 500;
constexpr float GREEN_KOOPA_SPAWN_TIME = 0xff * 1000.0f / 60.0f;
constexpr float KOOPA_RESPAWNING_TIME = 2000;

constexpr float KOOPA_WING_HOP = 0x01000 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float KOOPA_WING_BIG_HOP = 0x03000 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float KOOPA_WING_ACTIVATE_TIME = 2400;
constexpr float KOOPA_IGNORE_DAMAGE_TIME = 200;

#define KOOPA_STATE_NORMAL 1
#define KOOPA_STATE_WING 2
#define KOOPA_STATE_IN_SHELL 3
#define KOOPA_STATE_RESPAWNING 4
#define KOOPA_STATE_DEAD_BOUNCE 5

/*

GroundTroop_XVel:
    .byte -$08, $08
    .byte -$06, $06
    .byte -$05, $05

    .byte $F0, $10, $F4, $0C, $F7, $09
*/

class CKoopa : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

protected:
    LPGAMEOBJECT player = NULL;

    float ignoreDamageTimer = 0.0f;
    int hopCount = 0;
    float hopTimer = 0.0f;
    float changeDirTimer = 0.0f;
    float wingActivateTimer = KOOPA_WING_ACTIVATE_TIME;

    bool onGround = false;

    float holdYOffset = 0.0f;
    float respawnTimer = 0.f;

    int respawnNx = 1;
    bool isRed = false;

    CCollisionCast turnCast;
    CCollisionCast shellCast;
    CCollisionCast insideWallCast;

    int GetAnimationId();
    int GetAnimationRedId();
    int GetAnimationGreenId();
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
    CKoopa(float x, float y);
	int IsCollidable() override { return !isDeleted && state != KOOPA_STATE_DEAD_BOUNCE; };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return state != KOOPA_STATE_DEAD_BOUNCE; }
    int IsHeld() const { return player != NULL; }
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    void SetState(int state) override;

    bool IsInShell() const { return state == KOOPA_STATE_IN_SHELL; }
    void PlayerHit(int nx);
    void AttachHold(LPGAMEOBJECT player, float holdYOffset);
    void DetachHold();
    void TurnAround() { nx *= -1; }

    void DeadBounce() { SetState(KOOPA_STATE_DEAD_BOUNCE); }

    void SetHasWing() { SetState(KOOPA_STATE_WING); }
    void SetRedKoopa() { isRed = true; }

    void OnNoCollision(float dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;


	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};