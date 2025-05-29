#include "Firework.h"
#include <Engine/Loaders/SpritesLoader.h>
#include "ContentIds/Firework.h"
#include <Engine/Graphics/Animations.h>

bool CFirework::IsContentLoaded = false;
void CFirework::LoadContent()
{
	if (IsContentLoaded)
		return;
	SpritesLoader loader;
	loader.Load(FIREWORK_SPRITES_PATH);
}

void CFirework::Update(float dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (showDelay >= 0.0f)
		showDelay -= dt;
}

void CFirework::Render()
{
	CAnimations* const animations = CAnimations::GetInstance();
	if (showDelay <= 0)
		animations->Get(FIREWORK_ID_ANIMATION_NORMAL)->Render(position.x, position.y, 0.0f);
}
