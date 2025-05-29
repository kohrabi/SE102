#pragma once


#include "Engine/GameObject.h"

#include "Engine/Graphics/Animations.h"

constexpr float BOOMERANG_X_VELOCITY_LIMIT = 0x02000 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float BOOMERANG_X_ACCELERATION = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float BOOMERANG_Y_VELOCITY_LIMIT = 0x01200 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float BOOMERANG_Y_ACCELERATION = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;

/*
Boomerang_XVelDelta:	.byte $01, -$01
Boomerang_XVelLimit:	.byte $20, $E0
Boomerang_YVelAccel:	.byte $01, -$01
Boomerang_YVelLimit:	.byte $12, -$12
*/

class CBoomerang : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;

    CGameObject* parent;
    bool flip = false;
public:
    // Tile number xTile counting from 0
    // Tile number yTile counting from 0
    CBoomerang(float x, float y, int nx, CGameObject* parent) : CGameObject(x, y, 0.0f)
    {
        LoadContent();
        layer = SortingLayer::CORPSE;
        this->nx = nx;
        this->parent = parent;
        velocity.x = nx * BOOMERANG_X_ACCELERATION * 2.0f;
    }
    int IsBlocking() override { return 0; }
    int IsDirectionColliable(float nx, float ny) override { return 1; }

    void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
        left = position.x - 4;
        top = position.y - 4;
        right = position.x + 4;
        bottom = position.y + 4;
    }

    void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
    void Render() override;
};