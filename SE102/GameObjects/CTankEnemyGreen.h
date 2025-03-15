#pragma once

#include "CTankEnemy.h"

class CTankEnemyGreen : public CTankEnemy {
private:
	bool spawnNext = false;
public:
	CTankEnemyGreen(float x, float y, float rotation, float vx, float vy, float timeOffset) : CTankEnemy(x, y, rotation, vx, vy, timeOffset) {
		this->timeOffset = timeOffset;
		shootTimer = 1.5f;
	}

	int GetType() const override { return TYPE_ENEMY; }

	void OnDestroy() override;
};