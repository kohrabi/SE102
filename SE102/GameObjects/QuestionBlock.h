#pragma once

#include "Engine/GameObject.h"
#include "Engine/debug.h"

#include "Engine/CollisionCast.h"

#define QUESTION_BLOCK_SPAWN_COIN 1
#define QUESTION_BLOCK_SPAWN_LEAF 2
#define QUESTION_BLOCK_SPAWN_ONE_UP 3
#define QUESTION_BLOCK_ANIMATION_TIME 100
#define QUESTION_BLOCK_ANIMATION_Y_VEL 0.1f

class CQuestionBlock : public CGameObject {
private:
    int spawnType = -1;
	int spawnCount = 0;
	bool isActive = true;
	float animationTimer = 0.0f;
	float ogYPos = 0.0f;
	bool isHit = false;
	CCollisionCast cast;
	
	static bool IsContentLoaded;
	static void LoadContent();
public:

	CQuestionBlock(float x, float y, int type, int count);

	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
    
	void Hit(int dx);

	int GetSpawnType() const { return spawnType; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override {
		const float size = 16.0f / 2.0f;
		left = position.x - size;
		top = position.y - size;
		right = position.x + size;
		bottom = position.y + size;
	}
};