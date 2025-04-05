#pragma once
#include <unordered_map>
#include <d3dx10.h>

#include <string>
#include "Texture.h"

using namespace std;

#define STRING_TO_WSTRING(str) wstring(str.begin(), str.end())

/*
	Manage texture database
*/
class CTextures
{
	static CTextures * __instance;

	unordered_map<wstring, LPTEXTURE> textures;

public: 
	CTextures();
	void Add(wstring filePath);
	LPTEXTURE Get(const wstring& path);
	LPTEXTURE Get(const string& path);

	static CTextures * GetInstance();
};