#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define GOOMBA_INTRO_Y_VELOCITY 0.03

#define GOOMBA_X_SPEED 0x00A00 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_KILL_TIME 500

#define GOOMBA_WING_HOP 0x01000 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_WING_BIG_HOP 0x03000 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_WING_HOP_COUNT 4
#define GOOMBA_WING_HOP_TIME 140
#define GOOMBA_WING_ACTIVATE_TIME 1400
#define GOOMBA_WING_CHANG_DIR 1040
#define GOOMA_IGNORE_DAMAGE_TIME 200

#define GOOMBA_STATE_NORMAL 1
#define GOOMBA_STATE_WING 2
#define GOOMBA_STATE_DEAD 3
#define GOOMBA_STATE_DEAD_BOUNCE 4

class CGoomba : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    float killTimer = 0.0f;

    int hopCount = 0;
    float hopTimer = 0.0f;
    float changeDirTimer = 0.0f;
    float wingActivateTimer = GOOMBA_WING_ACTIVATE_TIME;
    float ignoreDamageTimer = 0.0f;

    bool isRedGoomba = false;

    bool onGround = false;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CGoomba(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent(); 
        nx = -1;
        layer = SortingLayer::NPC;
        SetState(GOOMBA_STATE_NORMAL);
    }
	int IsCollidable() override { 
        return !isDeleted && (state != GOOMBA_STATE_DEAD || state != GOOMBA_STATE_DEAD_BOUNCE); };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return state != GOOMBA_STATE_DEAD || state != GOOMBA_STATE_DEAD_BOUNCE; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 4; 
        top = position.y - 4 + 4; 
        right = position.x + 4; 
        bottom = position.y + 4 + 4; 
    }

    void SetState(int state) override;

    void SetRedGoomba() { isRedGoomba = true; }

    void SetHasWing() { SetState(GOOMBA_STATE_WING); }

    void SetKill() { SetState(GOOMBA_STATE_DEAD); }

    void DeadBounce() { SetState(GOOMBA_STATE_DEAD_BOUNCE); }

    void OnNoCollision(float dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};