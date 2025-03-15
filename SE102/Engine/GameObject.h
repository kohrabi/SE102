#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Engine/Graphics/Texture.h"
#include "Engine/Math/Vector2.h"


#define TYPE_GAME_OBJECT 1
#define TYPE_TANK 2
#define TYPE_PLAYER 3
#define TYPE_BULLET 4
#define TYPE_ENEMY 5


RECT GetTextureRegion(int x, int y, int xSize, int ySize);

class CGameObject
{
protected: 
	Vector2 position = Vector2::Zero;
	float rotation = 0.0f;
	bool destroy = false;
public: 
	void SetPosition(float x, float y, float rotation) { 
		this->position.x = x;
		this->position.y = y; 
		this->rotation = rotation; 
	}

	bool ShouldDestroy() { return destroy;  }
	Vector2 GetPosition() const { return position; }
	float GetRotation() const { return rotation; }
	virtual int GetType() const { return TYPE_GAME_OBJECT; }

	void Destroy() { 
		if (destroy)
			return;
		destroy = true; 
		OnDestroy();
	}
	void SetRotation(float newVal) { rotation = newVal; }

	CGameObject(float x = 0.0f, float y = 0.0f, float rotation = 0.0f);

	virtual void OnDestroy() {}
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};
typedef CGameObject * LPGAMEOBJECT;

class CMoveableObject : public CGameObject {
protected:
	Vector2 velocity;
public:
	CMoveableObject(float x, float y, float rotation, float vx, float vy) :CGameObject(x, y, rotation)
	{
		this->velocity.x = vx;
		this->velocity.y = vy;
	};
	virtual void Update(DWORD dt);
};