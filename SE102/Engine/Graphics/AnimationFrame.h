#pragma once

#include "Sprite.h"
#include "Engine/Math/Vector2.h"
#include <vector>

/*
	Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) : sprite(sprite), time(time) {}
	DWORD GetTime() { return time; }
	LPSPRITE const GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

