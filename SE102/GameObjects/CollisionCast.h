#pragma once

#include "Engine/GameObject.h"

class CCollisionCast : public CGameObject
{
private:
	int l, t, r, b;
public:
	int collisionCount = 0;
	CCollisionCast(Vector2 position, Vector2 size)
	{
		l = position.x - size.x / 2;
		t = position.y - size.y / 2;
		r = position.x + size.x / 2;
		b = position.y + size.y / 2;
	}
	CCollisionCast(int l, int t, int r, int b)
		: l(l), t(t), r(r), b(b) { }

	void Render() override {}

	void CheckOverlap(vector<LPGAMEOBJECT>* coObjects = NULL);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = l; top = t; right = r; bottom = b;
	}
};