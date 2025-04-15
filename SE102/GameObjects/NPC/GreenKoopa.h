#pragma once


#include "Engine/GameObject.h"


#include "Engine/Graphics/Animations.h"

#define GREEN_KOOPA_INTRO_Y_VELOCITY 0.03

#define GREEN_KOOPA_X_SPEED 0x00A00 * SUBSUBSUBPIXEL_DELTA_TIME
#define GREEN_KOOPA_SHELL_X_SPEED 0x02800 * SUBSUBSUBPIXEL_DELTA_TIME
#define GREEN_KOOPA_KILL_TIME 500
#define GREEN_KOOPA_SPAWN_TIME 8000
#define KOOPA_RESPAWNING_TIME 2000

#define KOOPA_STATE_NORMAL 1
#define KOOPA_STATE_IN_SHELL 2
#define KOOPA_STATE_RESPAWNING 3
#define KOOPA_STATE_DEAD_BOUNCE 4

class CGreenKoopa : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

protected:
    LPGAMEOBJECT player = NULL;
    float holdYOffset = 0.0f;
    float respawnTimer = 0.f;

    int respawnNx = 1;

    virtual int GetAnimationId();
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CGreenKoopa(float x, float y) : CGameObject(x, y, 0.0f) 
    { 
        LoadContent();
        nx = -1;
        layer = SortingLayer::NPC;
        SetState(KOOPA_STATE_NORMAL);
    }
	int IsCollidable() override { return !isDeleted && state != KOOPA_STATE_DEAD_BOUNCE; };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return state != KOOPA_STATE_DEAD_BOUNCE; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8; 
        top = position.y - 0; 
        right = position.x + 8; 
        bottom = position.y + 16; 
    }
    void SetState(int state) override;

    bool IsInShell() const { return state == KOOPA_STATE_IN_SHELL; }
    void PlayerHit(int nx);
    void AttachHold(LPGAMEOBJECT player, float holdYOffset);
    void DetachHold();
    void DeadBounce(bool kill)
    {
        SetState(KOOPA_STATE_DEAD_BOUNCE);
    }

    void OnNoCollision(float dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;


	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};