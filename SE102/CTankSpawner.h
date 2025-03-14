#pragma once

#include "GameObject.h"

class CTankSpawner : public CGameObject {
private:
	float spawnTime;
	float spawnTimer;
public:
	CTankSpawner(float x, float y, float rotation, float spawnTime, LPTEXTURE texture)
		: CGameObject(x, y, rotation, texture), spawnTime(spawnTime), spawnTimer(spawnTime)
	{
	};
	virtual void Update(DWORD dt);
};