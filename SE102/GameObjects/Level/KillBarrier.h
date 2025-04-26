#pragma once

#include "Engine/GameObject.h"

class CKillBarrier : public CGameObject
{
private:
	AABB rect;
public:
	CKillBarrier(float l, float t, float width, float height) : CGameObject(0, 0) { 
		rect.left = l; 
		rect.top = t;
		rect.right = l + width;
		rect.bottom = t + height;
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = rect.left;
		top = rect.top;
		right = rect.right;
		bottom = rect.bottom;
	}


	void Render() override { RenderBoundingBox(); }
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL);

	int IsCollidable() override { return 1; };
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) override { return 1; }
};