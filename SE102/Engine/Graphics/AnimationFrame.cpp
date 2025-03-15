#include "AnimationFrame.h"

void CAnimationFrame::Render(float x, float y)
{
	sprite->Draw(x, y);
	//for (const auto& subsprite : sprites) {
	//	subsprite.sprite->Draw(position.x + subsprite.offset.x, position.y + subsprite.offset.y);
	//}
}
