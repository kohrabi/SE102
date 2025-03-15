#pragma once

#include "Engine/GameObject.h"

class CTankSpawner : public CGameObject {
private:
	float spawnTime;
	float spawnTimer;
public:
	CTankSpawner(float x, float y, float rotation, float spawnTime)
		: CGameObject(x, y, rotation), spawnTime(spawnTime), spawnTimer(spawnTime)
	{
	};
	virtual void Update(DWORD dt);
	void Render() override {}
};