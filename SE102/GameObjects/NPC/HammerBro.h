#pragma once


#include "Engine/GameObject.h"
#include "Engine/CollisionCast.h"

#include "Engine/Graphics/Animations.h"

constexpr float HAMMER_BRO_X_SPEED = 0x00800 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float HAMMER_BRO_CHANGE_DIR_TIME = 0x40 * 1000.0f / 60.0f;
constexpr float HAMMER_BRO_STOP_TIME = 0x0F * 1000.0f / 60.0f;

constexpr float HAMMER_BRO_WING_BIG_HOP = 0x03000 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float HAMMER_BRO_TIMER_RELOAD = 256 * 1000.0f / 60.0f;
constexpr float HAMMER_BRO_RESET_THROW = 0x40 * 1000.0f / 60.0f;

#define HAMMER_BRO_STATE_NORMAL 1
#define HAMMER_BRO_STATE_DEAD_BOUNCE 2
#define HAMMER_BRO_STATE_DEAD 3

class CHammerBro : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

protected:
    CGameObject* player = NULL;
    float changeDirTimer = 0.0f;
    float stopTimer = 0.0f;
    int changeDirCount = 0;
    float throwTimer = HAMMER_BRO_TIMER_RELOAD;
    float resetThrownTimer = 0.0f;

    bool onGround = false;

    bool thrown = false;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
    CHammerBro(float x, float y);
	int IsCollidable() override { return !isDeleted && (state != HAMMER_BRO_STATE_DEAD_BOUNCE && state != HAMMER_BRO_STATE_DEAD); };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return (state != HAMMER_BRO_STATE_DEAD_BOUNCE && state != HAMMER_BRO_STATE_DEAD); }
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    void SetState(int state) override;

    void DeadBounce() { SetState(HAMMER_BRO_STATE_DEAD_BOUNCE); }
    void SetKill() { SetState(HAMMER_BRO_STATE_DEAD); }

    void OnNoCollision(float dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};