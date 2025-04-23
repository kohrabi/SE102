#pragma once

#include "Engine/GameObject.h"

#include "Engine/PlayScene.h"

class CPlayScene;
class CHUD : public CGameObject
{
private:

	static bool IsContentLoaded;
	static void LoadContent();

	int getNumberSpriteId(char number);

	CPlayScene* const currentScene;
public:
	CHUD(CPlayScene* const currentScene) : CGameObject(), currentScene(currentScene) { LoadContent(); }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override { left = 0; right = 0; top = 0; bottom = 0; }
	void Render() override;
	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;

	int IsCollidable() override { return false; };
	int IsBlocking() override { return 0; };
	int IsDirectionColliable(float nx, float ny) override { return 0; }
};