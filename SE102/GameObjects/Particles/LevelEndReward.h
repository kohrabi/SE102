#pragma once

#include "Engine/GameObject.h"

#define LEVEL_END_REWARD_STATE_NORMAL 1
#define LEVEL_END_REWARD_STATE_END 2

#define LEVEL_END_REWARD_NORMAL_TIMER 1500
#define LEVEL_END_REWARD_END_TIMER 1000

class CLevelEndReward : public CGameObject
{
private:
	static void LoadContent();
	static bool IsContentLoaded;

	int animationID = 0;
	float timer = 0.0f;
public:
	CLevelEndReward(float x, float y, int animationID);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override { left = 0; top = 0; right = 0; bottom = 0; }
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void SetState(int state) override;
	void OnDelete() override;

	int IsCollidable() override { return 0; };
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) override { return 0; }


};