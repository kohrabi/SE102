#pragma once

#include "CTankEnemy.h"

class CTankEnemyRed : public CTankEnemy {
private: 
	bool spawnNext = false;
public:
	CTankEnemyRed(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture, float timeOffset) : CTankEnemy(x, y, rotation, vx, vy, texture, timeOffset) {
		UpTextureRect = GetTextureRegion(0 + 8, 8, 16, 16);
		RightTextureRect = GetTextureRegion(6 + 8, 8, 16, 16);
		LeftTextureRect = GetTextureRegion(2 + 8, 8, 16, 16);
		DownTextureRect = GetTextureRegion(4 + 8, 8, 16, 16);
		this->timeOffset = timeOffset;
		lookDir = Vector2(-1, 0);
		//moveDir = Vector2(-1, 0);
		textureRegion = UpTextureRect;
		shootTimer = 1.5f;
	}

	int GetType() const override { return TYPE_ENEMY; }

	void OnDestroy() override;
};