#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define GOOMBA_INTRO_Y_VELOCITY 0.03

#define GOOMBA_X_SPEED 0x00A00 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_KILL_TIME 500

#define GOOMBA_WING_X_SPEED 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_WING_X_MAX_SPEED 0x01400 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_WING_HOP 0x01000 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_WING_BIG_HOP 0x03000 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_WING_HOP_COUNT 4
#define GOOMBA_WING_HOP_TIME 140 * 2
#define GOOMBA_WING_ACTIVATE 1070 * 2

#define GOOMBA_STATE_NORMAL 1
#define GOOMBA_STATE_DEAD 2
#define GOOMBA_STATE_DEAD_BOUNCE 3

class CGoomba : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    float killTimer = 0.0f;
    bool hasWing = true;
    int hopCount = 0;
    float hopTimer = 0.0f;
    float wingActivateTimer = GOOMBA_WING_ACTIVATE;
    bool wingActivate = false;
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
        return !isDeleted && state == GOOMBA_STATE_NORMAL; };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return state == GOOMBA_STATE_NORMAL; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8; 
        top = position.y - 8; 
        right = position.x + 8; 
        bottom = position.y + 8; 
    }

    void SetState(int state) override;

    void SetKill()
    {
        SetState(GOOMBA_STATE_DEAD);
    }

    void DeadBounce()
    {
        SetState(GOOMBA_STATE_DEAD_BOUNCE);
    }

    void OnNoCollision(float dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};