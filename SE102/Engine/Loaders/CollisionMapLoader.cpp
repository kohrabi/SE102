#include "CollisionMapLoader.h"

#include <iostream>
#include <fstream>
#include <string>
#include "Engine/Utils.h"

#define COLLISION_MAP_SECTION_UNKNOWN 0
#define COLLISION_MAP_SECTION_RECTANGLES 1
#define COLLISION_MAP_SECTION_MAP 2

void CollisionMapLoader::Load(string path) {
	ifstream f;
	f.open(path);

	int section = COLLISION_MAP_SECTION_UNKNOWN;
	string str = "";
	int currentMapLine = 0;
	while (getline(f, str)) {
		string line(str);

		if (line[0] == '#' || line == "") continue;	// skip comment lines	

		if (line == "[RECTANGLES]") { section = COLLISION_MAP_SECTION_RECTANGLES; continue; };
		if (line == "[MAP]") { currentMapLine = 0; section = COLLISION_MAP_SECTION_MAP; continue; };
		if (line[0] == '[') { section = COLLISION_MAP_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case COLLISION_MAP_SECTION_RECTANGLES: _ParseSection_RECTANGLES(line); break;
		case COLLISION_MAP_SECTION_MAP: _ParseSection_MAP(line, currentMapLine); break;
		default: DebugBreak(); break;
		}
	}
}

void CollisionMapLoader::_ParseSection_RECTANGLES(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() != 5)
		DebugBreak();
	RECT r;
	r.left = atoi(tokens[1].c_str()) * tileWidth;
	r.top = (atoi(tokens[2].c_str())) * tileHeight;
	r.right = (atoi(tokens[3].c_str()) + 1) * tileWidth;
	r.bottom = (atoi(tokens[4].c_str()) + 1) * tileHeight;
	collisionRegion.push_back(r);
}

void CollisionMapLoader::_ParseSection_MAP(string line, int& currentMapLine)
{
	vector<string> tokens = split(line);
	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i] != "")
			map[currentMapLine][i] = atoi(tokens[i].c_str());
	}
	currentMapLine++;
}
