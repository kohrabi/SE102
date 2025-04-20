#include "Helper.h"

#include <algorithm>

using namespace std;

float clampf(float value, float minV, float maxV) { return min(max(value, minV), maxV); }

int clampi(int value, int minV, int maxV) { return min(max(value, minV), maxV); }

int sign(float value) {
	if (value == 0.0f)
		return 0;
	if (value > 0.0f)
		return 1;
	return -1;
}

float move_towards(float value, float end, float delta) {
	if (value > end)
		return max(value - abs(delta), end);
	else // value <= end
		return min(value + abs(delta), end);
}