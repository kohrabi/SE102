#pragma once

#include <string>
#include <vector>
#include "Engine/Math/AABB.h"

using namespace std;

class CollisionMapLoader
{
private:
	void _ParseSection_RECTANGLES(string line);
	void _ParseSection_MAP(string line, int& currentMapLine);
public:
	vector<AABB> collisionRegion;
	int** map;
	int width = 0;
	int height = 0;
	int tileWidth = 0;
	int tileHeight = 0;
	CollisionMapLoader(int width, int height, int tileWidth, int tileHeight)
	{
		this->width = width;
		this->height = height;
		this->tileWidth = tileWidth;
		this->tileHeight = tileHeight;
		map = new int* [height];
		for (int i = 0; i < height; i++)
		{
			map[i] = new int[width];
		}
	}

	~CollisionMapLoader()
	{
		for (int i = 0; i < height; i++)
			delete[] map[i];
		delete[] map;
	}
	void Load(string path);
};