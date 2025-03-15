#include "Sprites.h"
#include "Engine/Game.h"
#include "Engine/debug.h"

CSprites * CSprites::__instance = NULL;

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprites::Add(int id, RECT r, LPTEXTURE tex)
{
	Add(id, r.left, r.top, r.right, r.bottom, tex);
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites.at(id);
}