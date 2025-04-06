#pragma once


#include "Engine/GameObject.h"

#include "contents.h"

#include "Engine/Graphics/Animations.h"

#define COIN_MAX_Y -18
#define COIN_DESTROY_TIME 200
#define COIN_Y_VELOCITY 0.17

class CCoin : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    long startTime;
    float maxYPos;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CCoin(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent(); 
        startTime = GetTickCount64() + COIN_DESTROY_TIME; 
        maxYPos = y + COIN_MAX_Y;
    }
	int IsCollidable() override { return false; };
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { left = 0; top = 0; right = 0; bottom = 0; }

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};