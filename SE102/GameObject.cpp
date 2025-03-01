#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

/*
	Initialize game object 
*/
CGameObject::CGameObject(float x, float y, LPTEXTURE tex)
{
	this->x = x;
	this->y = y;
	this->texture = tex;
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (texture != NULL) delete texture;
}

#define MARIO_VX 0.1f
#define MARIO_WIDTH 14

void CMario::Update(DWORD dt)
{
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	if (x <= 0 || x >= BackBufferWidth - MARIO_WIDTH) {
		
		vx = -vx;

		if (x <= 0)
		{
			x = 0;
		}
		else if (x >= BackBufferWidth - MARIO_WIDTH)
		{
			x = (float)(BackBufferWidth - MARIO_WIDTH);
		}
	}
	CMoveableObject::Update(dt);
}

void CEnemy::Update(DWORD dt)
{
	if (texture == NULL) return;
	CGame* game = CGame::GetInstance();

	int BackBufferHeight = game->GetBackBufferHeight();
	int BackBufferWidth = game->GetBackBufferWidth();
	int imageHeight = texture->getHeight();
	int imageWidth = texture->getWidth();

	CMoveableObject::Update(dt);
	if (y <= 0 || y >= BackBufferHeight + imageHeight) {

		if (y <= 0)
			y = (float)(BackBufferHeight + imageHeight);
		else if (y >= BackBufferHeight + imageHeight)
			y = 0;
	}
	if (x <= 0 || x >= BackBufferWidth + imageWidth) {

		if (x <= 0)
			x = (float)(BackBufferWidth + imageWidth);
		else if (x >= BackBufferWidth + imageWidth)
			x = 0;
	}
}

float clampf(float value, float minV, float maxV) { return min(max(value, minV), maxV); }

#define SHIP_VEL 0.02
#define SHIP_MAX_VEL 0.9
#define SHOOT_TIME 2
void CShip::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();

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
	
	vx = clampf(vx + inputX * SHIP_VEL, -SHIP_MAX_VEL * boost, SHIP_MAX_VEL * boost);
	vy = clampf(vy + inputY * SHIP_VEL, -SHIP_MAX_VEL * boost, SHIP_MAX_VEL * boost);
	vx *= 0.9f;
	vy *= 0.9f;

	int BackBufferHeight = game->GetBackBufferHeight();
	int BackBufferWidth = game->GetBackBufferWidth();
	int imageHeight = texture->getHeight();
	int imageWidth = texture->getWidth();

	if (y <= 0 || y >= BackBufferHeight + imageHeight) {
		if (y <= 0)
			y = (float)(BackBufferHeight + imageHeight);
		else if (y >= BackBufferHeight + imageHeight)
			y = 0;
	}
	if (x <= 0 || x >= BackBufferWidth + imageWidth) {
		if (x <= 0)
			x = (float)(BackBufferWidth + imageWidth);
		else if (x >= BackBufferWidth + imageWidth)
			x = 0;
	}

	if (shootTimer > 0) shootTimer -= dt;
	//DebugOut(L"%f", dt);
	if (game->keyState[VK_SPACE] && shootTimer <= 0.0f) {
		game->objects.push_back(new CBullet(x, y, 0, -0.5, game->texBullet));
		shootTimer = SHOOT_TIME;
	}
	CMoveableObject::Update(dt);
}

void CBullet::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();

	int BackBufferHeight = game->GetBackBufferHeight();
	int imageHeight = texture->getHeight();

	if (y <= 0 || y >= BackBufferHeight + imageHeight) {
		destroy = true;
	}
	CMoveableObject::Update(dt);
}

void CMoveableObject::Update(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}
