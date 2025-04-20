#pragma once

float clampf(float value, float minV, float maxV);
int clampi(int value, int minV, int maxV);
int sign(float value);
float move_towards(float value, float end, float delta);