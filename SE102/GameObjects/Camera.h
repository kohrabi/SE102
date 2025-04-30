#pragma once

#include "Engine/GameObject.h"

constexpr float DEADZONE_X = 10.0f;
constexpr float DEADZONE_Y = 15.0f;

class CCamera : public CGameObject
{
private:
	Vector2 deadzoneOffset;
public:
	CCamera() : CGameObject(0.0f, 0.0f){}

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override { left = 0.0f; top = 0.0f; right = 0.0f; bottom = 0.0f; }
	int IsCollidable() override { return 0; };
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) override { return 0; }
};