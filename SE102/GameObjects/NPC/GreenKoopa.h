#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define GREEN_KOOPA_INTRO_Y_VELOCITY 0.03

#define GREEN_KOOPA_X_SPEED 0x00F00 * SUBSUBSUBPIXEL_DELTA_TIME
#define GREEN_KOOPA_SHELL_X_SPEED 0x02800 * SUBSUBSUBPIXEL_DELTA_TIME
#define GREEN_KOOPA_KILL_TIME 500

#define GREEN_KOOPA_STATE_NORMAL 1

class CGreenKoopa : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

protected:
    LPGAMEOBJECT player = NULL;
    bool inShell = false;
    float holdYOffset = 0.0f;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CGreenKoopa(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent();
        nx = -1;
        layer = SortingLayer::NPC;
    }
	int IsCollidable() override { return !isDeleted; };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return true; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8; 
        top = position.y - 0; 
        right = position.x + 8; 
        bottom = position.y + 16; 
    }

    bool IsInShell() const { return inShell; }
    void PlayerHit(int nx);
    void AttachHold(LPGAMEOBJECT player, float holdYOffset);
    void DetachHold();

    void OnNoCollision(DWORD dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};