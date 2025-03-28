#pragma once

#include "CTankEnemy.h"

class CTankEnemyRed : public CTankEnemy {
private: 
	bool spawnNext = false;
public:
	CTankEnemyRed(float x, float y, float rotation, float timeOffset) : CTankEnemy(x, y, rotation, timeOffset) {
		this->timeOffset = timeOffset;
		lookDir = Vector2(-1, 0);
		//moveDir = Vector2(-1, 0);
		shootTimer = 1.5f;
	}

	void OnDestroy() override;
};