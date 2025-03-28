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
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override {}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
};