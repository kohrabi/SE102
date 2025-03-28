#pragma once

#include "CTank.h"
#include "Engine/GameObject.h"

#define MOVE_DIR_CHANGE_TIME 1.0f
#define SHOOT_TIME 5.f

class CTankEnemy : public CTank {
protected:
	float moveChangeDirTimer = 0.0f;
	float timeOffset = 0.0f;
	float shootTimer = 0.0f;
public:
	CTankEnemy(float x, float y, float rotation, float timeOffset) : CTank(x, y, rotation) {
		this->timeOffset = timeOffset;
		shootTimer = 1.5f;
		CAnimations* const animations = CAnimations::GetInstance();
		MoveUpAnimation = animations->Get(ENEMY_MOVEUP_ANIMATION);
		MoveRightAnimation = animations->Get(ENEMY_MOVERIGHT_ANIMATION);
		MoveLeftAnimation = animations->Get(ENEMY_MOVELEFT_ANIMATION);
		MoveDownAnimation = animations->Get(ENEMY_MOVEDOWN_ANIMATION);
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;

	static void LoadContent();
};