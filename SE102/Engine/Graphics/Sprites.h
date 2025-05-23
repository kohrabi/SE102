#pragma once
#include <Windows.h>
#include <unordered_map>

#include <d3dx10.h>

#include "Texture.h"
#include "Sprite.h"

using namespace std;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites * __instance;

	unordered_map<int, LPSPRITE> sprites;

public:

	CSprites() {}
	CSprites(const CSprites&) = delete;
	void operator=(const CSprites&) = delete;

	void Add(int id, const RECT& r, LPTEXTURE tex);
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex);
	LPSPRITE Get(int id);
	void Clear();

	static CSprites * GetInstance();
};

