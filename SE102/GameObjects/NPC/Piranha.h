#pragma once


#include "Engine/GameObject.h"

#include "Engine/Math/Vector2i.h"

#include "Engine/Graphics/Animations.h"

constexpr float FIRE_PIRANHA_Y_SPEED = 0x00A00 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float FIRE_PIRANHA_DOWN_POS = 33.0f;
#define FIRE_PIRANHA_UP_TIME 5000
#define FIRE_PIRANHA_WAIT_SHOOT_TIMER 1000
#define FIRE_PIRANHA_SHOOT_ANIMATION_TIME 500
#define FIRE_PIRANHA_MOVE_TIME 2000
#define FIRE_PIRANHA_DOWN_TIME 5000

#define FIRE_PIRANHA_DOWN 1
#define FIRE_PIRANHA_MOVE 2
#define FIRE_PIRANHA_UP 3
#define FIRE_PIRANHA_WAIT 4
class CPiranha : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    int GetFireAnimationId();

    Vector2 spawnPosition = Vector2::Zero;
    Vector2i lookDir = Vector2i::Zero;

    float timer = 0.0f;
    float shootTimer = 0.0f;
    bool canShoot = false;
    bool isFirePiranha = false;
    bool isUp = false;
    bool isGreen = false;
    int heightCount = 2;

    float shootAnimation = 0.0f;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
	CPiranha(float x, float y, int height, bool isGreen, bool isFire) : CGameObject(x, y, 0.0f) 
    { 
        this->heightCount = max(height, 1);
        this->isGreen = isGreen;
        this->isFirePiranha = isFire;

        LoadContent(); 
        nx = -1;
        spawnPosition = position;
        SetState(FIRE_PIRANHA_DOWN);
        timer /= 2.0f;

        position.y = y + FIRE_PIRANHA_DOWN_POS;
        layer = SortingLayer::NPC;
    }

    void OnDelete() override;
    void SetState(int state) override;
	int IsCollidable() override { return !isDeleted; };
    int IsBlocking() override { return false; }
    int IsDirectionColliable(float nx, float ny) override { return 1; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { 
        left = position.x - 8; 
        top = position.y - 10; 
        right = position.x + 8; 
        bottom = position.y + 16; 
    }

    void OnCollisionWith(LPCOLLISIONEVENT e) override;

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};