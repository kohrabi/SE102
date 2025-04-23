#include "Animation.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, float z, bool flipX, bool flipY)
{
	frames.at(currentFrame)->GetSprite()->Draw(x, y, z, flipX, flipY);
	ULONGLONG now = GetTickCount64();
	if (!stop) {
		if (currentFrame == -1)
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames.at(currentFrame)->GetTime();
			if (now - lastFrameTime > t * timeScale)
			{
				if (loop)
					currentFrame = (currentFrame + 1) % frames.size();
				else
				{
					if (currentFrame < frames.size() - 1)
						currentFrame = currentFrame + 1;
				}
				lastFrameTime = now;
				//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
			}
		}
	}

}

void CAnimation::RenderScreen(float x, float y, float z, bool flipX, bool flipY)
{
	frames.at(currentFrame)->GetSprite()->DrawScreen(x, y, z, flipX, flipY);
	ULONGLONG now = GetTickCount64();
	if (!stop) {
		if (currentFrame == -1)
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames.at(currentFrame)->GetTime();
			if (now - lastFrameTime > t * timeScale)
			{
				if (loop)
					currentFrame = (currentFrame + 1) % frames.size();
				else
				{
					if (currentFrame < frames.size() - 1)
						currentFrame = currentFrame + 1;

				}
				lastFrameTime = now;
				//if (currentFrame == frames.size()) currentFrame = 0;
				//DebugOut(L"now: %d, lastFrameTime: %d, t: %d\n", now, lastFrameTime, t);
			}
		}
	}
}

