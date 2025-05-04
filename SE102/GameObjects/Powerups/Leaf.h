 #pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

constexpr float LEAF_X_SPEED = 0x00200 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float LEAF_X_LIMIT = 0x02000 * SUBSUBSUBPIXEL_DELTA_TIME;

constexpr float LEAF_Y_VELS = -0x04000 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float LEAF_TIMER_VALS = 0x10 * 1000.0f / 60.0f;

#define LEAF_STATE_INTRO 1
#define LEAF_STATE_NORMAL 2
/*
    ; Values used by the leaf based on bounce direction
Leaf_YVels:	.byte $04, $E0, $E0, $E0
Leaf_TimerVals:	.byte $08, $FF, $FF, $FF

Leaf_XVelByOsc:	.byte $02, -$02
Leaf_XVelLimit:	.byte $20, -$20

    
Bouncer_PUpVel:.byte $00, -$40, -$40, -$30, -$20, -$10, $00, $10, $20, $30, $40
*/


class CLeaf : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    long startTime;
    float leafTimer = 0.0f;
    bool leafStop = false;
    float maxYPos;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
    CLeaf(float x, float y);
	int IsCollidable() override { return !isDeleted; };
    int IsBlocking() override { return false; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 4; 
        top = position.y - 8; 
        right = position.x + 4; 
        bottom = position.y + 8; 
    }

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};