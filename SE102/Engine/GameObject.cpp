#include <d3dx9.h>

#include "debug.h"
#include "Engine/Game.h"
#include "Engine/GameObject.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "contents.h"
#include "Graphics/Textures.h"
#include "Math/AABB.h"

#include "Engine/Game.h"

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

float GetLayer(SortingLayer layer, int orderInLayer)
{
	return (float)layer / (float)SORTING_LAYER_SIZE + 0.001f * orderInLayer;
}

bool CGameObject::CompareSortingLayer(const LPGAMEOBJECT a, const LPGAMEOBJECT b)
{
	return GetLayer(a->GetSortingLayer(), a->GetOrderInLayer()) < GetLayer(b->GetSortingLayer(), b->GetOrderInLayer());
}

void CGameObject::RenderBoundingBox()
{
	return;
	D3DXVECTOR3 p(position.x, position.y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(TEXTURE_PATH_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l + rect.right / 2, t + rect.bottom / 2, 0.0f, 1.0f, bbox, rect.left, rect.top, rect.right, rect.bottom, false, false, 0.25f);
}

bool CGameObject::IsColliderInCamera()
{
	CGame* const game = CGame::GetInstance();
	if (game != NULL)
	{
		constexpr float BUFFER = 100.0f;
		AABB cam = game->GetCameraBound();
		AABB collider;
		cam.left -= BUFFER;
		cam.right += BUFFER;
		cam.top -= BUFFER;
		cam.bottom += BUFFER;

		GetBoundingBox(collider.left, collider.top, collider.right, collider.bottom);

		return CCollision::CheckAABBOverlaps(cam, collider);
	}
	return false;
}