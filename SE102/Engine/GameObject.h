#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Engine/Graphics/Texture.h"
#include "Engine/Math/Vector2.h"
#include "Collision.h"

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

#define SUBPIXEL 1.0F/16.0F
#define MAX_DELTA_TIME 60.f / 1000.0f
#define SUBSUBSUBPIXEL SUBPIXEL * SUBPIXEL * SUBPIXEL
#define SUBSUBSUBPIXEL_DELTA_TIME SUBSUBSUBPIXEL * MAX_DELTA_TIME

#define OBJECT_FALL 0x00300 * SUBSUBSUBPIXEL_DELTA_TIME
#define OBJECT_MAX_FALL 0x04000 * SUBSUBSUBPIXEL_DELTA_TIME

RECT GetTextureRegion(int x, int y, int xSize, int ySize);

class CGameObject;
class CGame;

class CGameObject
{
protected:
	Vector2 position = Vector2::Zero;
	Vector2 velocity;

	float rotation = 0.0f;

	bool isDeleted = false;

	int nx;
	int state;
public:
	void SetPosition(float x, float y) { position.x = x; position.y = y; }
	void SetVelocity(float vx, float vy) { velocity.x = vx, velocity.y = vy; }

	bool IsDeleted() { return isDeleted; }
	Vector2 GetPosition() const { return position; }
	Vector2 GetVelocity() { return velocity; }
	float GetRotation() const { return rotation; }
	int GetState() const { return state; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;

	void SetRotation(float newVal) { rotation = newVal; }
	virtual void SetState(int state) { this->state = state; }

	void Delete() {
		if (isDeleted)
			return;
		isDeleted = true;
		OnDelete();
	}

	CGameObject(float x = 0.0f, float y = 0.0f, float rotation = 0.0f);

	virtual void OnDelete() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {}
	virtual void Render() = 0;
	void RenderBoundingBox();


	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable();

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};

	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	// Is this object collide with other object at certain direction ( like ColorBox )
	virtual int IsDirectionColliable(float nx, float ny);
};
typedef CGameObject* LPGAMEOBJECT;