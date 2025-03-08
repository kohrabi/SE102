#pragma once

#include "CTank.h"

class CPlayer : public CTank {
public:
	CPlayer(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture) : CTank(x, y, rotation, vx, vy, texture) {
		UpTextureRect = GetTextureRegion(0, 0, 16, 16);
		RightTextureRect = GetTextureRegion(6, 0, 16, 16);
		LeftTextureRect = GetTextureRegion(2, 0, 16, 16);
		DownTextureRect = GetTextureRegion(4, 0, 16, 16);
		textureRegion = UpTextureRect;
		isPlayer = true;
	}
	int GetType() const override { return TYPE_PLAYER; }

	void Update(DWORD dt) override;
};