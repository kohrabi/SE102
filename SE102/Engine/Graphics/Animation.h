#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

#include <vector>
#include "Engine/debug.h"

using namespace std;

class CAnimation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	float timeScale = 1.0f;
	vector<LPANIMATION_FRAME> frames;
	bool stop = false;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = 0; }
	const LPANIMATION_FRAME& getCurrentAnimationFrame() { 
		ASSERT(currentFrame >= 0 && currentFrame < frames.size(), "How can i get the current frame. Frames is empty");
		return frames[currentFrame]; 
	}

	void SetTimeScale(float newValue) { timeScale = newValue; }

	void Add(int spriteId, DWORD time = 0);
	void Stop() { stop = true; }
	void Play() { stop = false; }
	void Reset() { currentFrame = 0; }
	void Render(float x, float y, bool flipX = false, bool flipY = false);
};

typedef CAnimation* LPANIMATION;