#pragma once

#include "CTank.h"
#include "GameObject.h"

#define MOVE_DIR_CHANGE_TIME 1.0f
#define SHOOT_TIME 5.f

class CTankEnemy : public CTank {
protected:
	float moveChangeDirTimer = 0.0f;
	float timeOffset = 0.0f;
	float shootTimer = 0.0f;
public:
	CTankEnemy(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture, float timeOffset) : CTank(x, y, rotation, vx, vy, texture) {
		UpTextureRect = GetTextureRegion(0 + 8, 0, 16, 16);
		RightTextureRect = GetTextureRegion(6 + 8, 0, 16, 16);
		LeftTextureRect = GetTextureRegion(2 + 8, 0, 16, 16);
		DownTextureRect = GetTextureRegion(4 + 8, 0, 16, 16);
		this->timeOffset = timeOffset;
		textureRegion = UpTextureRect;
		shootTimer = 1.5f;
	}
	int GetType() const override { return TYPE_ENEMY; }

	void Update(DWORD dt) override;
};