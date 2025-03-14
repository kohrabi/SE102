#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>

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

#define MARIO_VX 0.1f
#define MARIO_WIDTH 14

void CMario::Update(DWORD dt)
{
	int BackBufferWidth = CGame::GetInstance()->GetViewportWidth();
	if (position.x <= 0 || position.x >= BackBufferWidth - MARIO_WIDTH) {
		
		velocity.x = -velocity.x;

		if (position.x <= 0)
		{
			position.x = 0;
		}
		else if (position.x >= BackBufferWidth - MARIO_WIDTH)
		{
			position.x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}
	CMoveableObject::Update(dt);
}

void CEnemy::Update(DWORD dt)
{
	if (texture == NULL) return;
	CGame* const game = CGame::GetInstance();

	int BackBufferHeight = game->GetViewportHeight();
	int BackBufferWidth = game->GetViewportWidth();
	int imageHeight = texture->getHeight();
	int imageWidth = texture->getWidth();

	CMoveableObject::Update(dt);
	if (position.y <= 0 || position.y >= BackBufferHeight + imageHeight) {

		if (position.y <= 0)
			position.y = (float)(BackBufferHeight + imageHeight);
		else if (position.y >= BackBufferHeight + imageHeight)
			position.y = 0;
	}
	if (position.x <= 0 || position.x >= BackBufferWidth + imageWidth) {

		if (position.x <= 0)
			position.x = (float)(BackBufferWidth + imageWidth);
		else if (position.x >= BackBufferWidth + imageWidth)
			position.x = 0;
	}
}

float clampf(float value, float minV, float maxV) { return min(max(value, minV), maxV); }

#define SHIP_VEL 0.02f
#define SHIP_MAX_VEL 0.9f
#define SHOOT_TIME 0.02f
void CShip::Update(DWORD dt)
{
	CGame* const game = CGame::GetInstance();

	float inputX = 0;
	float inputY = 0;
	if (game->keyState[VK_LEFT])
		inputX = -1;
	else if (game->keyState[VK_RIGHT])
		inputX = 1;

	if (game->keyState[VK_UP]) 
		inputY = -1;
	else if (game->keyState[VK_DOWN])
		inputY = 1;
	float boost = 1.0f;
	if (game->keyState[VK_SHIFT]) {
		boost = 3.f;
	}
	
	velocity.x = clampf(velocity.x + inputX * SHIP_VEL, -SHIP_MAX_VEL * boost, SHIP_MAX_VEL * boost);
	velocity.y = clampf(velocity.y + inputY * SHIP_VEL, -SHIP_MAX_VEL * boost, SHIP_MAX_VEL * boost);
	velocity.x *= 0.9f;
	velocity.y *= 0.9f;

	rotation += M_PI / 15.0f;

	int BackBufferHeight = game->GetViewportHeight();
	int BackBufferWidth = game->GetViewportWidth();
	int imageHeight = texture->getHeight();
	int imageWidth = texture->getWidth();

	if (position.y <= 0 || position.y >= BackBufferHeight + imageHeight) {
		if (position.y <= 0)
			position.y = (float)(BackBufferHeight + imageHeight);
		else if (position.y >= BackBufferHeight + imageHeight)
			position.y = 0;
	}
	if (position.x <= 0 || position.x >= BackBufferWidth + imageWidth) {
		if (position.x <= 0)
			position.x = (float)(BackBufferWidth + imageWidth);
		else if (position.x >= BackBufferWidth + imageWidth)
			position.x = 0;
	}

	if (shootTimer > 0) shootTimer -= dt / 1000.f;
	if (game->keyState[VK_SPACE] && shootTimer <= 0.0f) {
		DebugOut(L"%f %f\n", position.x, position.y);
		game->objects.push_back(new CBullet(position.x, position.y, 0.0f, 0, -0.5f, game->texBullet));
		shootTimer = SHOOT_TIME;
	}
	CMoveableObject::Update(dt);
}

void CBullet::Update(DWORD dt)
{
	CGame* const game = CGame::GetInstance();

	int BackBufferHeight = game->GetViewportHeight();
	int imageHeight = texture->getHeight();
	/*
	if (position.y <= 0 || position.y >= BackBufferHeight + imageHeight) {
		destroy = true;
	}*/
	CMoveableObject::Update(dt);
}

void CMoveableObject::Update(DWORD dt)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
}

#include "CTankEnemy.h"
void CTankBullet::Update(DWORD dt)
{
	if (destroy)
		return;
	CGame* const game = CGame::GetInstance();
	int backBufferHeight = game->GetViewportHeight();
	int imageHeight = texture->getWidth();
	if (textureRegion.bottom - textureRegion.top != 0)
		imageHeight = textureRegion.bottom - textureRegion.top;
	if (position.y <= 0 || position.y >= backBufferHeight - imageHeight) {
		destroy = true;
	}

	int backBufferWidth = game->GetViewportWidth();
	int imageWidth = texture->getWidth();
	if (textureRegion.right - textureRegion.left != 0)
		imageWidth = textureRegion.right - textureRegion.left;
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
