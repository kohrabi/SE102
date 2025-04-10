#pragma once

#include "Engine/GameObject.h"

class CCollisionCast : public CGameObject
{
private:
	float l = 0, t = 0, r = 0, b = 0;
	LPGAMEOBJECT parent = NULL;
public:
	int collisionCount = 0;
	CCollisionCast() {}
	CCollisionCast(Vector2 position, Vector2 size)
		: parent(parent)
	{
		l = position.x - size.x / 2.0f;
		t = position.y - size.y / 2.0f;
		r = position.x + size.x / 2.0f;
		b = position.y + size.y / 2.0f;
	}
	CCollisionCast(float l, float t, float r, float b)
		: l(l), t(t), r(r), b(b), parent(parent) { }

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
	void SetParent(LPGAMEOBJECT parent)
	{
		this->parent = parent;
	}

	void Render() override { RenderBoundingBox(); }

	void CheckOverlap(vector<LPGAMEOBJECT>* coObjects = NULL);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = l; top = t; right = r; bottom = b;
	}
};