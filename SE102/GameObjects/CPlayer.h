#pragma once

#include "CTank.h"

class CPlayer : public CTank {
public:
	CPlayer(float x, float y, float rotation) : CTank(x, y, rotation) {
		isPlayer = true;
		CAnimations* const animations = CAnimations::GetInstance();
		MoveUpAnimation = animations->Get(PLAYER_MOVEUP_ANIMATION);
		MoveRightAnimation = animations->Get(PLAYER_MOVERIGHT_ANIMATION);
		MoveLeftAnimation = animations->Get(PLAYER_MOVELEFT_ANIMATION);
		MoveDownAnimation = animations->Get(PLAYER_MOVEDOWN_ANIMATION);
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;

	static void LoadContent();
};