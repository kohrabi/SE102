#include "Boomerang.h"
#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/Boomerang.h"

bool CBoomerang::IsContentLoaded = false;
void CBoomerang::LoadContent()
{
	if (IsContentLoaded) return;
	IsContentLoaded = true;

	SpritesLoader loader;
	loader.Load(BOOMERANG_SPRITES_PATH);
}

void CBoomerang::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (abs(parent->GetPosition().x - position.x) <= 16.0f && flip)
		Delete();

	velocity.x = clampf(velocity.x + BOOMERANG_X_ACCELERATION * nx, -BOOMERANG_X_VELOCITY_LIMIT, BOOMERANG_X_VELOCITY_LIMIT);
	velocity.y = clampf(velocity.y + BOOMERANG_Y_ACCELERATION * (flip ? 1 : -1), -BOOMERANG_Y_VELOCITY_LIMIT, BOOMERANG_Y_VELOCITY_LIMIT);

	if (abs(velocity.x) == BOOMERANG_X_VELOCITY_LIMIT) {
		nx *= -1;
		flip = true;
	}

	position.x += velocity.x * dt;
	position.y += velocity.y * dt;
	//if (!IsColliderInCamera())
		//Delete();
}

void CBoomerang::Render()
{
	CAnimations* const animations = CAnimations::GetInstance();
	animations->Get(BOOMERANG_ID_ANIMATION_BOOMERANG)->Render(position.x, position.y, GetLayer(layer, 0));
}
