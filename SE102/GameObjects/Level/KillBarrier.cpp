#include "KillBarrier.h"
#include <GameObjects/Mario.h>

void CKillBarrier::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKillBarrier::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
	{
		dynamic_cast<CMario*>(e->obj)->Kill();
	}
	else
		e->obj->Delete();
}
