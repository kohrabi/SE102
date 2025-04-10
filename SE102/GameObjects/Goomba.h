#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define GOOMBA_INTRO_Y_VELOCITY 0.03

#define GOOMBA_X_SPEED 0x00A00 * SUBSUBSUBPIXEL_DELTA_TIME
#define GOOMBA_KILL_TIME 500

#define GOOMBA_STATE_NORMAL 1

class CGoomba : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    bool kill = false;
    float killTimer = 0.0f;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CGoomba(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent(); 
        nx = -1;
    }
	int IsCollidable() override { 
        return !isDeleted && !kill; };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { 
        return !kill; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8; 
        top = position.y - 8; 
        right = position.x + 8; 
        bottom = position.y + 8; 
    }

    void SetKill()
    {
        kill = true;
        killTimer = GOOMBA_KILL_TIME;
    }

    void OnNoCollision(DWORD dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};