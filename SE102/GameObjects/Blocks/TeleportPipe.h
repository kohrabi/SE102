#pragma once

#include "Engine/GameObject.h"
#include "Engine/debug.h"

#include "Engine/CollisionCast.h"

class CTeleportPipe : public CGameObject {
private:
	bool isHit = false;
	Vector2 size;
	Vector2 teleportTo;
	int directionY = 0;
public:

	CTeleportPipe(float x, float y, float width, float height, Vector2 teleportTo, int directionY) 
		: CGameObject(x, y, 0.0f), size(width, height), teleportTo(teleportTo), directionY(directionY) {}
	Vector2 GetTeleportPosition() const { return teleportTo; }
	Vector2 GetSize() const { return size; }
	int GetDirectionY() const { return directionY; }

	void Render() override { RenderBoundingBox(); }
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) {}
	int IsCollidable() override { return 1; };
	int IsBlocking() override { return 0; };
	int IsDirectionColliable(float nx, float ny) override { return 1; };


	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		left = position.x - round(size.x / 2.0f);
		top = position.y - round(size.y / 2.0f);
		right = position.x + round(size.x / 2.0f);
		bottom = position.y + round(size.y / 2.0f);
	}
};