#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"


using namespace std;

class CAnimation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
	bool stop = false;
public:
	CAnimation(int defaultTime) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = 0; }
	const LPANIMATION_FRAME& getCurrentAnimationFrame() { 
		if (frames.size() <= currentFrame) {
			throw new exception("How can i get the current frame. Frames is empty");
			DebugBreak();
		}
		return frames[currentFrame]; 
	}
	void Add(int spriteId, DWORD time = 0);
	void Stop() { stop = true; }
	void Play() { stop = false; }
	void Reset() { currentFrame = 0; }
	void Render(float x, float y);
};

typedef CAnimation* LPANIMATION;