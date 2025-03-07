#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"
#include "Vector2.h"

class CGameObject
{
protected: 
	Vector2 position = Vector2::Zero;
	float rotation = 0.0f;
	bool destroy = false;

	// This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
	// For now, just a pointer to a single texture
	LPTEXTURE texture;			
public: 
	void SetPosition(float x, float y, float rotation) { 
		this->position.x = x;
		this->position.y = y; 
		this->rotation = rotation; 
	}

	bool ShouldDestroy() { return destroy;  }
	Vector2 GetPosition() const { return position; }
	float GetRotation() const { return rotation; }

	void SetRotation(float newVal) { rotation = newVal; }

	CGameObject(float x = 0.0f, float y = 0.0f, float rotation = 0.0f, LPTEXTURE texture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render();

	~CGameObject();
};
typedef CGameObject * LPGAMEOBJECT;

class CMoveableObject : public CGameObject {
protected:
	Vector2 velocity;
public:
	CMoveableObject(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture) :CGameObject(x, y, rotation, texture)
	{
		this->velocity.x = vx;
		this->velocity.y = vy;
	};
	virtual void Update(DWORD dt);
};

class CBrick : public CGameObject
{
public:
	CBrick(float x, float y, float rotation, LPTEXTURE texture): CGameObject(x,y, rotation,texture) {}
	void Update(DWORD dt) {}; 
};

class CMario : public CMoveableObject
{
public:
	CMario(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture) :CMoveableObject(x, y, rotation, vx, vy, texture)
	{
	};
	void Update(DWORD dt) override;
};

class CShip: public CMoveableObject
{
	float shootTimer = 0.0f;
public:
	CShip(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture) :CMoveableObject(x, y, rotation, vx, vy, texture)
	{
	};
	void Update(DWORD dt) override;
};

class CBullet : public CMoveableObject
{
	float radius;
public:
	CBullet(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture) :CMoveableObject(x, y, rotation, vx, vy, texture)
	{
		this->radius = texture->getWidth() * 1.0f;
	};
	void Update(DWORD dt) override;
};

class CEnemy : public CMoveableObject
{
public:
	CEnemy(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture) : CMoveableObject(x, y, rotation, vx, vy, texture)
	{
		this->velocity.x = vx;
		this->velocity.y = vy;
		float width = texture->getWidth() / 2.0f;
		float height = texture->getHeight() / 2.0f;
		//aabb.bottomLeftX = x - width;
		//aabb.bottomLeftY = y + height;
		//aabb.topRightX = x + width;
		//aabb.topRightY = y - height;
	};
	void Update(DWORD dt) override;
};
