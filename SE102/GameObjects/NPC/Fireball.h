#pragma once


#include "Engine/GameObject.h"

#include "Engine/Graphics/Animations.h"

constexpr float FIREBALL_VELOCITY = 0x00B00 * SUBSUBSUBPIXEL_DELTA_TIME;

class CFireball : public CGameObject {
private:
    static void LoadContent();
    static bool IsContentLoaded;
public:
    // Tile number xTile counting from 0
    // Tile number yTile counting from 0
    CFireball(float x, float y, int nx, int ny) : CGameObject(x, y, 0.0f)
    {
        LoadContent();
        layer = SortingLayer::NPC;
        velocity = Vector2((float)nx, (float)ny).normalized() * FIREBALL_VELOCITY;
    }
    int IsDirectionColliable(float nx, float ny) override { return 0; }

    void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
        left = position.x - 8;
        top = position.y - 8;
        right = position.x + 8;
        bottom = position.y + 8;
    }

    void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
    void Render() override;
};