#include "CollisionCast.h"


void CCollisionCast::CheckOverlap(vector<LPGAMEOBJECT>* coObjects)
{
	collisionCount = 0;
	if (coObjects == NULL)
		return;
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (!obj->IsCollidable() || obj == parent || obj->GetVelocity().length() > 0)
			continue;
		AABB objRect;
		obj->GetBoundingBox(objRect.left, objRect.top, objRect.right, objRect.bottom);
		if (objRect.right - objRect.left <= 0)
			continue;

		AABB rect;
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