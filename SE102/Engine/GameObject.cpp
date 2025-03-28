#include <d3dx9.h>

#include "debug.h"
#include "Engine/Game.h"
#include "Engine/GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameObjects/CTankEnemy.h"

/*
	Initialize game object 
*/
CGameObject::CGameObject(float x, float y, float rotation)
{
	this->position.x = x;
	this->position.y = y;
	this->rotation = rotation;
}

RECT GetTextureRegion(int xTile, int yTile, int xSize, int ySize)
{
	RECT r;
	r.left = xTile * xSize;
	r.right = (xTile + 1) * xSize - 1;
	r.top = (yTile) * ySize;
	r.bottom = (yTile + 1) * ySize - 1;
	return r;
}
