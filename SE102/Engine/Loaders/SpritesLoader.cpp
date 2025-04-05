#include "SpritesLoader.h"

#include <vector>
#include <fstream>

#include "Engine/Utils.h"
#include "Engine/Graphics/Textures.h"
#include "Engine/Graphics/Sprites.h"
#include "Engine/Graphics/Animations.h"
#include "Engine/debug.h"

void SpritesLoader::_ParseSection_SPRITES(const string& line) {
    vector<string> row = split(line);

	if (row.size() != 2) return;

    vector<string> tokens = split(row[1], " ");
    
	if (tokens.size() != 5) return;

	int ID = atoi(row[0].c_str());

	int l = atoi(tokens[0].c_str());
	int t = atoi(tokens[1].c_str());
	int r = atoi(tokens[2].c_str());
	int b = atoi(tokens[3].c_str());
	string texAlias = alias.at(tokens[4].c_str());

	CTextures::GetInstance()->Add(STRING_TO_WSTRING(texAlias));
	LPTEXTURE tex = CTextures::GetInstance()->Get(texAlias);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %s not found!\n", texAlias.c_str());
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void SpritesLoader::_ParseSection_ALIAS(const string& line) {
    vector<string> row = split(line);
	if (row.size() != 2) return;

    alias[row[1]] = row[0];
}


void SpritesLoader::_ParseSection_ANIMATIONS(const string& line)
{
	vector<string> row = split(line);

	if (row.size() != 2) return;

    vector<string> tokens = split(row[1], " ");

	if (tokens.size() < 2) return;

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(row[0].c_str());
	for (int i = 0; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void SpritesLoader::Load(LPCWSTR path)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", path);
	ifstream f;
	f.open(path);
    int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
        string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ALIAS]") { section = ASSETS_SECTION_ALIAS; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = ASSETS_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
        case ASSETS_SECTION_ALIAS: _ParseSection_ALIAS(line); break;
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
        default: DebugBreak(); break;
		}
    }
}