#pragma once

#include <Windows.h>
#include <d3dx10.h>

#include "Texture.h"
#include "Vector2.h"


#define TYPE_GAME_OBJECT 1
#define TYPE_TANK 2
#define TYPE_PLAYER 3
#define TYPE_BULLET 4
#define TYPE_ENEMY 5

class CGameObject
{
protected: 
	Vector2 position = Vector2::Zero;
	float rotation = 0.0f;
	bool destroy = false;
	RECT textureRegion = {};

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
	virtual int GetType() const { return TYPE_GAME_OBJECT; }

	void Destroy() { 
		if (destroy)
			return;
		destroy = true; 
		OnDestroy();
	}
	void SetTextureRegion(int l, int r, int t, int b) {
		textureRegion.left = l;
		textureRegion.right = r;
		textureRegion.top = t;
		textureRegion.bottom = b;

	}
	void SetRotation(float newVal) { rotation = newVal; }

	CGameObject(float x = 0.0f, float y = 0.0f, float rotation = 0.0f, LPTEXTURE texture = NULL);

	virtual void OnDestroy() {}
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
		float width = texture->getWidth() / 2.0f;
		float height = texture->getHeight() / 2.0f;
		//aabb.bottomLeftX = x - width;
		//aabb.bottomLeftY = y + height;
		//aabb.topRightX = x + width;
		//aabb.topRightY = y - height;
	};
	void Update(DWORD dt) override;
};

class CTankBullet : public CMoveableObject {
private: 
	bool isPlayer = false;
public:
	CTankBullet(float x, float y, float rotation, float vx, float vy, LPTEXTURE texture, bool player = false) : CMoveableObject(x, y, rotation, vx, vy, texture)
	{
		textureRegion.left = 322;
		textureRegion.right = 325;
		textureRegion.top = 101;
		textureRegion.bottom = 106;
		isPlayer = player;
	};

	int GetType() const override { return TYPE_BULLET; }
	void Update(DWORD dt) override;
};

RECT GetTextureRegion(int x, int y, int xSize, int ySize);