#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define COIN_MAX_Y -18
#define COIN_DESTROY_TIME 200
#define COIN_Y_VELOCITY 0.17f

#define COIN_STATE_INTRO 1
#define COIN_STATE_NORMAL 2

class CCoin : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    ULONGLONG startTime;
    float maxYPos;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CCoin(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent(); 
        SetState(COIN_STATE_NORMAL);
    }

    void SetState(int state) override;


	int IsCollidable() override { return true; };
    int IsBlocking() override { return false; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8;
        top = position.y - 8; 
        right = position.x + 8;
        bottom = position.y + 8; 
    }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};