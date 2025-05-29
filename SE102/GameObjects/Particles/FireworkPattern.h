#pragma once

#include "Engine/Math/Vector2.h"
#include <vector>
#include <string>

using namespace std;

struct FireworkPattern {
	Vector2 Center;
	vector<Vector2> Positions;
};

static FireworkPattern FireworkPatterns[3];

void SpawnFireworkPattern(int pattern, Vector2 center);
void LoadFireworkPattern(int pattern, string map);
void LoadFireworkPatterns();