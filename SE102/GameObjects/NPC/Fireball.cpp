#include "Fireball.h"
#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/Fireball.h"

bool CFireball::IsContentLoaded = false;
void CFireball::LoadContent()
{
	if (IsContentLoaded) return;
	IsContentLoaded = true;

	SpritesLoader loader;
	loader.Load(FIREBALL_SPRITES_PATH);
}

void CFireball::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	if (!IsColliderInCamera())
		Delete();
}

void CFireball::Render()
{
	CAnimations* const animations = CAnimations::GetInstance();
	animations->Get(FIREBALL_ID_ANIMATION_FIREBALL)->Render(position.x, position.y, GetLayer(layer, 0));
}
