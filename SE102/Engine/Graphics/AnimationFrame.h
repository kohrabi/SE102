#pragma once

#include "Sprite.h"
#include "Engine/Math/Vector2.h"
#include <vector>

/*
	Sprite animation
*/

struct Subsprite {
	LPSPRITE sprite;
	Vector2 offset;
};

class CAnimationFrame
{
	LPSPRITE sprite;
	//std::vector<Subsprite> sprites;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) : sprite(sprite), time(time) {}
	DWORD GetTime() { return time; }
	LPSPRITE const GetSprite() { return sprite; }
	//std::vector<Subsprite> GetSprites() { return sprites; }

	//void AddSprite(LPSPRITE sprite, Vector2 offset) {
	//	sprites.emplace_back(sprite, offset);
	//}

	//float GetFrameWidth() const {
	//	float width = 0.f;
	//	for (int i = 0; i < sprites.size(); i++) {
	//		const auto& subsprite = sprites[i];
	//		width += subsprite.sprite->getWidth() / 2.0f;
	//	}
	//	return width;
	//}

	virtual void Render(float x, float y);
};

typedef CAnimationFrame* LPANIMATION_FRAME;

