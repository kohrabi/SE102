#pragma once

#include "Engine/GameObject.h"

class CTankBullet : public CMoveableObject {
private:
	bool isPlayer = false;
public:
	CTankBullet(float x, float y, float rotation, float vx, float vy, bool player = false) : CMoveableObject(x, y, rotation, vx, vy)
	{
		isPlayer = player;
	};

	int GetType() const override { return TYPE_BULLET; }
	void Update(DWORD dt) override;
	void Render() override;
};