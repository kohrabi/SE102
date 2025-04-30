#include "CollisionCast.h"
#include <Engine/Graphics/Textures.h>
#include <contents.h>

#include "Engine/Game.h"


void CCollisionCast::CheckOverlap(vector<LPGAMEOBJECT>* coObjects)
{
	collision.clear();
	if (coObjects == NULL)
		return;
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (!obj->IsCollidable())
			continue;
		AABB objRect;
		obj->GetBoundingBox(objRect.left, objRect.top, objRect.right, objRect.bottom);
		if (objRect.right - objRect.left <= 0)
			continue;
		if (checkCondition != nullptr && !checkCondition(obj))
			continue;
		//if (obj == parent || obj->GetVelocity().length() > 0)
		//	continue;
		AABB rect;
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		if (CCollision::CheckAABBOverlaps(rect, objRect))
		{
			collision.push_back(obj);
		}
	}
}


void CCollisionCast::RenderBoundingBox()
{
	return;
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(TEXTURE_PATH_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l + rect.right / 2, t + rect.bottom / 2, 0.0f, 1.0f, bbox, rect.left, rect.top, rect.right, rect.bottom, false, false, 0.25f);
}