#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define MUSHROOM_INTRO_Y_VELOCITY 0.03f

#define MUSHROOM_X_SPEED 0x01000 * SUBSUBSUBPIXEL_DELTA_TIME

#define MUSHROOM_STATE_INTRO 1
#define MUSHROOM_STATE_NORMAL 2
/*

Bouncer_PUpVel:.byte $00, -$40, -$40, -$30, -$20, -$10, $00, $10, $20, $30, $40
*/

class CMushroom : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;
protected:
    long startTime;
    float maxYPos;
    bool inIntro = false;
public:
	CMushroom(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent(); 
        state = MUSHROOM_STATE_INTRO;
        maxYPos = y - 16;
        nx = 1;
        inIntro = true;
        layer = SortingLayer::NPC;
    }
	int IsCollidable() override { return !isDeleted && !inIntro; };
    int IsBlocking() override { return false; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 4; 
        top = position.y - 8; 
        right = position.x + 4; 
        bottom = position.y + 8; 
    }

    void OnNoCollision(float dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};