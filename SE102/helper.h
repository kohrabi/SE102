#pragma once

float clampf(float value, float minV, float maxV) { return min(max(value, minV), maxV); }