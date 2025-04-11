#pragma once

struct Tile
{
	string texturePath;
	Vector2 position;
	RECT textureRegion;

	Tile() {}

	Tile(string texturePath, Vector2 position, RECT textureRegion)
	{
		this->texturePath = texturePath;
		this->position = position;
		this->textureRegion = textureRegion;
	}
};