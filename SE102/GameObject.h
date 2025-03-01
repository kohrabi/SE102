#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"

class CGameObject
{
protected: 
	float x; 
	float y;
	bool destroy = false;

	// This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
	// For now, just a pointer to a single texture
	LPTEXTURE texture;			
public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	bool ShouldDestroy() { return destroy;  }
	float GetX() { return x; }
	float GetY() { return y; }

	CGameObject(float x = 0.0f, float y=0.0f, LPTEXTURE texture = NULL);

	virtual void Update(DWORD dt) = 0;
	virtual void Render();

	~CGameObject();
};
typedef CGameObject * LPGAMEOBJECT;

class CMoveableObject : public CGameObject {
protected:
	float vx;
	float vy;
public:
	CMoveableObject(float x, float y, float vx, float vy, LPTEXTURE texture) :CGameObject(x, y, texture)
	{
		this->vx = vx;
		this->vy = vy;
	};
	virtual void Update(DWORD dt);
};

class CBrick : public CGameObject
{
public:
	CBrick(float x, float y, LPTEXTURE texture): CGameObject(x,y,texture) {}
	void Update(DWORD dt) {}; 
};

class CMario : public CMoveableObject
{
public: 
	CMario(float x, float y, float vx, float vy, LPTEXTURE texture) :CMoveableObject(x, y, vx, vy, texture)
	{
	};
	void Update(DWORD dt) override;
};

class CShip: public CMoveableObject
{
	float shootTimer = 0.0f;
public:
	CShip(float x, float y, float vx, float vy, LPTEXTURE texture) :CMoveableObject(x, y, vx, vy, texture)
	{
	};
	void Update(DWORD dt) override;
};

class CBullet : public CMoveableObject
{
	float radius;
public:
	CBullet(float x, float y, float vx, float vy, LPTEXTURE texture) :CMoveableObject(x, y, vx, vy, texture)
	{
		this->radius = texture->getWidth();
	};
	void Update(DWORD dt) override;
};

struct AABB {
	float topRightX;
	float topRightY;
	float bottomLeftX;
	float bottomLeftY;
};
class CEnemy : public CMoveableObject
{
	AABB aabb;
public:
	CEnemy(float x, float y, float vx, float vy, LPTEXTURE texture) : CMoveableObject(x, y, vx, vy, texture)
	{
		this->vx = vx;
		this->vy = vy;
		float width = texture->getWidth() / 2;
		float height = texture->getHeight() / 2;
		aabb.bottomLeftX = x - width;
		aabb.bottomLeftY = y + height;
		aabb.topRightX = x + width;
		aabb.topRightY = y - height;
	};
	void Update(DWORD dt) override;
};
