#pragma once

#define REWARD_FRAMES_EMPTY 0
#define REWARD_FRAMES_MUSHROOM 1
#define REWARD_FRAMES_FLOWER 2
#define REWARD_FRAMES_STAR 3

struct GameGlobals
{
	int marioLife = 4;
	int nextEmptySlot = 0;
	int rewardFrames[3]{ 0, 0, 0 };
};