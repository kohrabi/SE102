#pragma once

#include "Engine/GameObject.h"

class CCollisionCast : public CGameObject
{
private:
	float l, t, r, b;
public:
	int collisionCount = 0;
	CCollisionCast() {}
	CCollisionCast(Vector2 position, Vector2 size)
	{
		l = position.x - size.x / 2;
		t = position.y - size.y / 2;
		r = position.x + size.x / 2;
		b = position.y + size.y / 2;
	}
	CCollisionCast(float l, float t, float r, float b)
		: l(l), t(t), r(r), b(b) { }

	void SetBoundingBox(Vector2 position, Vector2 size)
	{
		l = position.x - size.x / 2;
		t = position.y - size.y / 2;
		r = position.x + size.x / 2;
		b = position.y + size.y / 2;
	}

	void SetBoundingBox(float l, float t, float r, float b)
	{
		this->l = l;
		this->t = t;
		this->r = r;
		this->b = b;
	}

	void Render() override { RenderBoundingBox(); }

	void CheckOverlap(vector<LPGAMEOBJECT>* coObjects = NULL);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = l; top = t; right = r; bottom = b;
	}
};