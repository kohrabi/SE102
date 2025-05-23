#include <Windows.h>

#include "Engine/debug.h"
#include "Engine/Game.h"
#include "textures.h"
#include "Engine/Utils.h"
#include <string>

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
	if (textures.find(filePath) != textures.end())
		return;
	textures[filePath] = CGame::GetInstance()->LoadTexture(filePath.c_str());
}

LPTEXTURE CTextures::Get(const wstring& path)
{
	return textures.at(path);
}


LPTEXTURE CTextures::Get(const string& path)
{
	return Get(ToWSTR(path));
}



