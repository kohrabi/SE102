#pragma once
#include "Game.h"
#include "Scene.h"
#include "Graphics/Textures.h"
#include "Engine/Loaders/CollisionMapLoader.h"
#include "GameObjects/TileLayer.h"

#include <string>
#include <unordered_map>

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/LayerGroup.hpp>

using namespace std;
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 			
	void _ParseSection_PROPERTIES(string line);
public: 
	CPlayScene(int id, wstring filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void LoadMap(string path);
	void LoadLayers(CTextures* const textures, const tmx::Map& tMap, const std::vector<tmx::Layer::Ptr>& layers, 
		const vector<tmx::Tileset>& tilesets, const CollisionMapLoader& collisionLoader, const vector<LPTILELAYER>& collisionObjects);
	void Clear();
	void PurgeDeletedObjects();
	

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

