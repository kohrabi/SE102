#include "CollisionCast.h"


void CCollisionCast::CheckOverlap(vector<LPGAMEOBJECT>* coObjects)
{
	if (coObjects == NULL)
		return;
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (!obj->IsCollidable())
			continue;
		float objl, objt, objr, objb;
		obj->GetBoundingBox(objl, objt, objr, objb);
		if (objr - objl <= 0)
			continue;
		RECT objRect;
		objRect.left = objl;
		objRect.top = objt;
		objRect.right = objr;
		objRect.bottom = objb;

		RECT rect;
		rect.left = l;
		rect.top = t;
		rect.right = r;
		rect.bottom = b;
		if (CCollision::CheckAABBOverlaps(rect, objRect))
		{
			collisionCount++;
		}
	}
}