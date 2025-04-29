#pragma once

#include "Engine/GameObject.h"

#include <functional>

class CCollisionCast
{
private:
	float l = 0, t = 0, r = 0, b = 0;
	function<bool(LPGAMEOBJECT)> checkCondition;
public:
	vector<LPGAMEOBJECT> collision = vector<LPGAMEOBJECT>();
	CCollisionCast() {}
	CCollisionCast(Vector2 position, Vector2 size)
	{
		l = position.x - size.x / 2.0f;
		t = position.y - size.y / 2.0f;
		r = position.x + size.x / 2.0f;
		b = position.y + size.y / 2.0f;
	}
	CCollisionCast(float l, float t, float r, float b)
		: l(l), t(t), r(r), b(b) { }

	void SetBoundingBox(Vector2 position, Vector2 size)
	{
		l = position.x - size.x / 2.0f;
		t = position.y - size.y / 2.0f;
		r = position.x + size.x / 2.0f;
		b = position.y + size.y / 2.0f;
	}

	void SetBoundingBox(float l, float t, float r, float b)
	{
		this->l = l;
		this->t = t;
		this->r = r;
		this->b = b;
	}

	void SetConditionFunction(function<bool(LPGAMEOBJECT)> checkCondition)
	{
		this->checkCondition = checkCondition;
	}

	void RenderBoundingBox();

	void CheckOverlap(vector<LPGAMEOBJECT>* coObjects);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = l; top = t; right = r; bottom = b;
	}
};