#include <Windows.h>

#include "Engine/debug.h"
#include "Engine/Game.h"
#include "textures.h"

CTextures * CTextures::__instance = NULL;

CTextures::CTextures()
{

}

CTextures *CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(wstring filePath)
{
	textures[filePath] = CGame::GetInstance()->LoadTexture(filePath.c_str());
}

LPTEXTURE CTextures::Get(const wstring& filePath)
{
	return textures.at(filePath);
}



