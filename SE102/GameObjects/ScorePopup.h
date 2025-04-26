#pragma once

#include "Engine/GameObject.h"

constexpr float SCORE_POPUP_RISE_VELOCITY = 0x02000 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float SCORE_POPUP_GRAVITY = 0x00100 * SUBSUBSUBPIXEL_DELTA_TIME;
constexpr float SCORE_POPUP_DESTROY_TIMER = 800;

enum ScoreType
{
	Score100,
	Score200,
	Score400,
	Score800,
	Score1000,
	Score2000,
	Score4000,
	Score8000,
	OneUp
};

class CScorePopup : public CGameObject
{
private:
	static void LoadContent();
	static bool IsContentLoaded;
	float destroyTimer = SCORE_POPUP_DESTROY_TIMER;
	ScoreType scoreType = ScoreType::Score100;
public:
	CScorePopup(float x, float y, ScoreType scoreType = ScoreType::Score100);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override { left = 0; top = 0; right = 0; bottom = 0; }
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual int IsDirectionColliable(float nx, float ny) { return 0; }


};