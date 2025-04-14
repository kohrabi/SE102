#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define FIRE_PIRANHA_INTRO_Y_VELOCITY 0.03f

#define FIRE_PIRANHA_Y_SPEED 0.03f
#define FIRE_PIRANHA_UP_TIME 5000
#define FIRE_PIRANHA_MOVE_TIME 2000
#define FIRE_PIRANHA_DOWN_TIME 5000

#define FIRE_PIRANHA_DOWN 1
#define FIRE_PIRANHA_MOVE 2
#define FIRE_PIRANHA_UP 3
#define FIRE_PIRANHA_WAIT 4
class CFirePiranha : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    Vector2 ogPosition = Vector2::Zero;
    float timer = 0;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CFirePiranha(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent(); 
        nx = -1;
        ogPosition = position;
        SetState(FIRE_PIRANHA_DOWN);
        position.y = y + 32.0f;
        layer = SortingLayer::NPC;
    }
    void SetState(int state) override;
	int IsCollidable() override { return !isDeleted; };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return false; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8; 
        top = position.y - 16; 
        right = position.x + 8; 
        bottom = position.y + 16; 
    }

    void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};