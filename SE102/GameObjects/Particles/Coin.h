#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define COIN_KILL_TIME 550
constexpr float COIN_INIT_Y_VEL = 0x07A00 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float COIN_Y_DESTROY = 0x02000 * SUBSUBSUBPIXEL;

#define COIN_STATE_INTRO 1
#define COIN_STATE_NORMAL 2
#define COIN_STATE_BRICK 3

class CCoin : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    CGameObject* brick;
    float killTimer = 0.0f;
    bool noScore = false;
    float maxYPos;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CCoin(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent(); 
        SetState(COIN_STATE_NORMAL);
        layer = SortingLayer::BLOCK;
    }

    void SetState(int state) override;

    void OnDelete() override;

    void SetNoScore() { noScore = true; }
	int IsCollidable() override { return state != COIN_STATE_BRICK; };
    int IsBlocking() override { return false; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8;
        top = position.y - 8; 
        right = position.x + 8;
        bottom = position.y + 8; 
    }
    void SwitchToBrick();
    void SwitchToCoin();

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};