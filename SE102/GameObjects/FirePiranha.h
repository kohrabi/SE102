#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define FIRE_PIRANHA_INTRO_Y_VELOCITY 0.03

#define FIRE_PIRANHA_Y_SPEED 0.1
#define FIRE_PIRANHA_UP_TIME 5000
#define FIRE_PIRANHA_MOVE_TIME 500
#define FIRE_PIRANHA_DOWN_TIME 5000

#define FIRE_PIRANHA_UP 1
#define FIRE_PIRANHA_MOVE 2
#define FIRE_PIRANHA_WAIT 3
#define FIRE_PIRANHA_DOWN 4
class CFirePiranha : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    float timer = 0;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CFirePiranha(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent(); 
        nx = -1;
        position.y = y - 16.0f;
    }
	int IsCollidable() override { return !isDeleted; };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return false; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8; 
        top = position.y - 16; 
        right = position.x + 8; 
        bottom = position.y + 16; 
    }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};