#pragma once

#include "CTankEnemy.h"

class CTankEnemyGreen : public CTankEnemy {
private:
	bool spawnNext = false;
public:
	CTankEnemyGreen(float x, float y, float rotation, float timeOffset) : CTankEnemy(x, y, rotation, timeOffset) {
		this->timeOffset = timeOffset;
		shootTimer = 1.5f;
	}

	void OnDelete() override;
};