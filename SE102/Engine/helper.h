#pragma once
#include <utility>

float clampf(float value, float minV, float maxV) { return std::min(std::max(value, minV), maxV); }