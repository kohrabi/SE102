#pragma once

#include "CTank.h"

class CPlayer : public CTank {
public:
	CPlayer(float x, float y, float rotation, float vx, float vy) : CTank(x, y, rotation, vx, vy) {
		isPlayer = true;
		CAnimations* const animations = CAnimations::GetInstance();
		MoveUpAnimation = animations->Get(PLAYER_MOVEUP_ANIMATION);
		MoveRightAnimation = animations->Get(PLAYER_MOVERIGHT_ANIMATION);
		MoveLeftAnimation = animations->Get(PLAYER_MOVELEFT_ANIMATION);
		MoveDownAnimation = animations->Get(PLAYER_MOVEDOWN_ANIMATION);
	}
	int GetType() const override { return TYPE_PLAYER; }

	void Update(DWORD dt) override;
	void Render() override;

	static void LoadPlayerContent();
};