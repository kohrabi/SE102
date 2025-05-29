#pragma once

#include "Engine/GameObject.h"

constexpr float DEADZONE_X = 10.0f;
constexpr float DEADZONE_Y = 15.0f;
constexpr float SHAKE_TIME = 10 * 1000.0f / 60.0f;
constexpr float SHAKE_FRAME_TIME = 1 * 1000.0f / 60.0f;

class CCamera : public CGameObject
{
private:
	CGameObject* followObject;
	Vector2 deadzoneOffset;
	float shakeFrameTimer = 0.0f;
	float shakeTimer = 0.0f;
public:
	CCamera() : CGameObject(0.0f, 0.0f) { nx = 1; }

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void Shake(float time = SHAKE_TIME)
	{
		shakeTimer = time;
	}

	void SetFollowObject(CGameObject* object) { followObject = object; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override { left = 0.0f; top = 0.0f; right = 0.0f; bottom = 0.0f; }
	int IsCollidable() override { return 0; };
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) override { return 0; }
};