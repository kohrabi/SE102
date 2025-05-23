#pragma once
#include "Game.h"
#include "Scene.h"
#include "Graphics/Textures.h"
#include "Engine/Loaders/CollisionMapLoader.h"
#include "GameObjects/TileObjects/CollidableTileLayer.h"
#include "GameObjects/Camera.h"

#include <string>
#include <unordered_map>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <GameObjects/HUD.h>

using namespace std;
//#include "Koopas.h"

#define LEVEL_TIME 300000

enum LevelState {
	Normal,
	Outro,
};

class CHUD;
class CPlayScene: public CScene
{
protected: 

	// A play scene has to have player, right? 			
	void _ParseSection_PROPERTIES(string line);
	CHUD* hud;
	CCamera* camera = NULL;
	bool stopTimer = false;
	float levelTimer = 0.0f;
	float outroLevelTimer = 0.0f;
	LevelState levelState = LevelState::Normal;

	vector<AABB> levelBounds;
public: 
	CPlayScene(int id, wstring filePath);

	float GetLevelTime() const { return levelTimer; }
	const vector<AABB>& GetLevelBounds() const { return levelBounds; }

	virtual void Load();
	virtual void Update(float dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	CCamera* GetCamera() { return camera; }
	void SetSceneState(LevelState state) { this->levelState = state; }
	void SetStopTimer(bool newVal) { this->stopTimer = true; }

	void LoadMap(string path);
	void LoadLayers(CTextures* const textures, const tmx::Map& tMap, const std::vector<tmx::Layer::Ptr>& layers, 
		const vector<tmx::Tileset>& tilesets, const CollisionMapLoader& collisionLoader, const vector<LPCOLLIDABLETILELAYER>& collisionObjects, int& i);
	void Clear();
	void PurgeDeletedObjects();
	

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

