#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "CTankEnemy.h"

/*
	Initialize game object 
*/
CGameObject::CGameObject(float x, float y, float rotation, LPTEXTURE tex)
{
	this->position.x = x;
	this->position.y = y;
	this->rotation = rotation;
	this->texture = tex;
}

void CGameObject::Render()
{
	if (textureRegion.right - textureRegion.left == 0) {
		CGame::GetInstance()->Draw(position.x, position.y, rotation, texture);
	}
	else {
		CGame::GetInstance()->Draw(position.x, position.y, rotation, texture, &textureRegion);
	}

}

CGameObject::~CGameObject()
{}

void CMoveableObject::Update(DWORD dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
}

void CTankBullet::Update(DWORD dt)
{
	if (destroy)
		return;
	CGame* const game = CGame::GetInstance();
	int backBufferHeight = game->GetViewportHeight();
	int imageHeight = texture->getWidth();
	if (textureRegion.bottom - textureRegion.top != 0)
		imageHeight = textureRegion.bottom - textureRegion.top;
	imageHeight /= 2;
	if (position.y <= 0 || position.y >= backBufferHeight - imageHeight) {
		destroy = true;
	}

	int backBufferWidth = game->GetViewportWidth();
	int imageWidth = texture->getWidth();
	if (textureRegion.right - textureRegion.left != 0)
		imageWidth = textureRegion.right - textureRegion.left;
	imageWidth /= 2;
	if (position.x <= 0 || position.x >= backBufferWidth - imageWidth) {
		destroy = true;
	}

	if (!destroy) {
		size_t size = game->objects.size();
		for (int i = 0; i < size; i++) {
			auto objectA = game->objects[i];
			if (objectA->ShouldDestroy() || objectA == this)
				continue;
			if (objectA->GetType() == TYPE_ENEMY) {
				auto tank = (CTankEnemy*)objectA;
				if (tank->IsPlayer() == isPlayer)
					continue;
				if (tank->checkPointInside(position)) {
					tank->Destroy();
					Destroy();
					break;
				}
			}
			if (objectA->GetType() == TYPE_BULLET && (objectA->GetPosition() - position).length() <= 1.0f)
			{
				DebugOut(L"%f\n", (objectA->GetPosition() - position).length());
				Destroy();
				objectA->Destroy();
				break;
			}
		}
	}

	CMoveableObject::Update(dt);
}

RECT GetTextureRegion(int x, int y, int xSize, int ySize)
{
	RECT r;
	r.left = x * xSize;
	r.right = (x + 1) * xSize - 1;
	r.top = (y) * ySize;
	r.bottom = (y + 1) * ySize - 1;
	return r;
}
