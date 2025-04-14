#include <iostream>
#include <fstream>
#include "contents.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Graphics/Sprites.h"
#include "debug.h"
#include "Graphics/Animation.h"
#include "Graphics/Animations.h"

#include "GameObjects/TileObjects/Tile.h"
#include "GameObjects/TileObjects/CollidableTile.h"
#include "GameObjects/Mario.h"
#include "GameObjects/QuestionBlock.h"
#include "Engine/Helper.h"
#include "GameObjects/OneWay.h"
#include "GameObjects/Coin.h"
#include "GameObjects/NPC/Goomba.h"
#include "GameObjects/NPC/FirePiranha.h"
#include "GameObjects/NPC/GreenKoopa.h"
#include "GameObjects/NPC/RedKoopa.h"

#include "GameObjects/TileObjects/CollidableTileLayer.h"

using namespace std;

CPlayScene::CPlayScene(int id, wstring filePath):
	CScene(id, filePath)
{
	player = NULL;
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_PROPERTIES 1

#define MAX_SCENE_LINE 1024

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath.c_str());

	ifstream f;
	f.open(sceneFilePath.c_str());

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[PROPERTIES]") { section = SCENE_SECTION_PROPERTIES; continue; };

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_PROPERTIES: _ParseSection_PROPERTIES(line); break;
		}
	}

	CTextures* const textures = CTextures::GetInstance();
	textures->Add(L"Content/menu.png");

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath.c_str());
}

void CPlayScene::Update(float dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		LPGAMEOBJECT obj = objects[i];
		if (obj->IsCollidable() && obj->IsColliderInCamera())
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	Vector2 position = player->GetPosition();

	CGame* const game = CGame::GetInstance();
	position.x -= game->GetBackBufferWidth() / 2;
	position.y -= game->GetBackBufferHeight() / 2;
	position.y = 500.f;

	position.x = clampf(position.x, levelBounds.left, levelBounds.right - game->GetBackBufferWidth());
	position.y = clampf(position.y, levelBounds.top, levelBounds.bottom - game->GetBackBufferHeight());

	CGame::GetInstance()->SetCamPos(position.x, position.y /*cy*/);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	CTextures* const textures = CTextures::GetInstance();
	CGame* const game = CGame::GetInstance();
	float cx, cy;
	game->GetCamPos(cx, cy);

	cy += 72.0f / 2.0f;
	game->SetCamPos(cx, cy);

	if (!is_sorted(objects.begin(), objects.end(), CGameObject::CompareSortingLayer))
	{
		vector<LPGAMEOBJECT> sortedObjects{ objects.begin(), objects.end() };
		sort(sortedObjects.begin(), sortedObjects.end(), CGameObject::CompareSortingLayer);
		for (int i = 0; i < sortedObjects.size(); i++)
			sortedObjects[i]->Render();
	}
	else
	{
		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();
	}


	game->DrawScreen(256.0f / 2.0f - 8.f, game->GetBackBufferHeight() - (60.0f - 48.0f), 0.0f, 1.0f, textures->Get(L"Content/menu.png"));
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

void CPlayScene::LoadMap(string path) {
	CGame* const game = CGame::GetInstance();
	CTextures* const textures = CTextures::GetInstance();
	tmx::Map tMap;
	if (tMap.load(path)) {
		const auto& tilesets = tMap.getTilesets();
		
		string collisionMapFilePath = "";
		for (const auto& property : tMap.getProperties())
		{
			if (property.getName() == "collisionMap")
			{
				collisionMapFilePath = tMap.getWorkingDirectory() + "/" + property.getFileValue();
			}
		}

		vector<LPCOLLIDABLETILELAYER> wallObjects = vector<LPCOLLIDABLETILELAYER>();
		CollisionMapLoader loader(tMap.getTileCount().x, tMap.getTileCount().y, tMap.getTileSize().x, tMap.getTileSize().y);
		if (collisionMapFilePath != "")
		{
			loader.Load(collisionMapFilePath);
			for (AABB region : loader.collisionRegion)
			{
				LPCOLLIDABLETILELAYER tileLayer = new CCollidableTileLayer(region);
				wallObjects.push_back(tileLayer);
				objects.push_back(tileLayer);
			}
		}
		
		for (const auto& tileset : tilesets) {
			textures->Add(STRING_TO_WSTRING(tileset.getImagePath()));
		}

		tmx::Colour color = tMap.getBackgroundColour();
		backgroundColor = D3DXCOLOR(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

		const auto& layers = tMap.getLayers();

		
		int order = 0;
		LoadLayers(textures, tMap, layers, tilesets, loader, wallObjects, order);

		const auto& bounds = tMap.getBounds();

		levelBounds.left = bounds.left;
		levelBounds.right = (bounds.left + bounds.width);
		levelBounds.top = bounds.top;
		levelBounds.bottom = (bounds.top + bounds.height);
	}
}

void CPlayScene::LoadLayers(CTextures* const textures, const tmx::Map& tMap, const std::vector<tmx::Layer::Ptr>& layers, 
		const vector<tmx::Tileset>& tilesets, const CollisionMapLoader& collisionLoader, const vector<LPCOLLIDABLETILELAYER>& collisionObjects, int& i)
{
	for (const auto& layer : layers) {
		if (layer->getType() == tmx::Layer::Type::Object)
		{
			const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
			const auto& layerObjects = objectLayer.getObjects();
			for (const auto& layerObject : layerObjects)
			{
				const auto& aabb = layerObject.getAABB();
				Vector2 position = Vector2::Zero;
				position = Vector2(aabb.left + aabb.width / 2.0f, aabb.top - aabb.height / 2.0f);
				if (layerObject.getClass() == "CMario")
				{
					ASSERT(player == NULL, "There are two player in one scene");
					CMario* player = new CMario(position.x, position.y);
					objects.push_back(player);
					this->player = player;
				}
				else if (layerObject.getClass() == "CQuestionBlock") 
				{
					int type = -1;
					int spawnCount = -1;
					for (const auto& property : layerObject.getProperties()) 
					{
						if (property.getName() == "type") {
							type = property.getIntValue();
						}
						else if (property.getName() == "spawnCount") {
							spawnCount = property.getIntValue();
						}
					}
					ASSERT(type != -1, "Question Block type is missing");
					CQuestionBlock* questionBlock = new CQuestionBlock(position.x, position.y, type, spawnCount);
					objects.push_back(questionBlock);
				}
				else if (layerObject.getClass() == "COneWay") 
				{
					position = Vector2(aabb.left + aabb.width / 2.0f, aabb.top + aabb.height / 2.0f);
					COneWay* questionBlock = new COneWay(position.x, position.y, aabb.width, aabb.height);
					objects.push_back(questionBlock);
				}
				else if (layerObject.getClass() == "CCoin")
				{
					CCoin* coin = new CCoin(position.x, position.y);
					objects.push_back(coin);
				}
				else if (layerObject.getClass() == "CGoomba")
				{
					CGoomba* obj = new CGoomba(position.x, position.y);
					objects.push_back(obj);
				}
				else if (layerObject.getClass() == "CFirePiranha")
				{
					CFirePiranha* obj = new CFirePiranha(position.x, position.y);
					objects.push_back(obj);
				}
				else if (layerObject.getClass() == "CGreenKoopa")
				{
					CGreenKoopa* obj = new CGreenKoopa(position.x, position.y);
					objects.push_back(obj);
				}
				else if (layerObject.getClass() == "CRedKoopa")
				{

					CRedKoopa* obj = new CRedKoopa(position.x, position.y);
					objects.push_back(obj);
				}
			}
		}
		else if (layer->getType() == tmx::Layer::Type::Tile)
		{
			const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
			const auto& layerSize = tileLayer.getSize();
			const auto& mapTileSize = tMap.getTileSize();
			const auto& mapSize = tMap.getTileCount();
			const auto& tiles = tileLayer.getTiles();
			CTileLayer* objTileLayer = new CTileLayer();
			objTileLayer->SetOrderInLayer(i);
			objects.push_back(objTileLayer);

			for (const auto& tileset : tilesets) {
				const auto& tilesetTiles = tileset.getTiles();
				for (size_t i = 0; i < mapSize.x; i++)
				{
					for (size_t j = 0; j < mapSize.y; j++) {
						size_t idx = j * mapSize.x + i;
						if (idx < tiles.size() && tiles[idx].ID >= tileset.getFirstGID() && tiles[idx].ID <= tileset.getLastGID()) {

							auto imagePosition = tilesetTiles[tiles[idx].ID - tileset.getFirstGID()].imagePosition;
							auto tileSize = tilesetTiles[tiles[idx].ID - tileset.getFirstGID()].imageSize;
							Vector2 offset(tileSize.x / 2.0f, tileSize.y / 2.0f);
							Vector2 position(mapTileSize.x * i + offset.x, mapTileSize.y * j + offset.y);
							// ERROR: TODO: THIS MIGHT BE WRONG
							if (tileSize.x != tileSize.y)
							{
								position.y -= offset.y;
							}
							RECT textureRegion = GetTextureRegion(imagePosition.x / mapTileSize.x, imagePosition.y / mapTileSize.y, tileSize.x, tileSize.y);

							if (layer->getName() != "Wall") {
								objTileLayer->AddTile(Tile(tileset.getImagePath(), position, textureRegion));
							}
							else {
								if (collisionObjects.size() > 0 && collisionLoader.map[j][i] - 1 > 0)
								{
									collisionObjects[collisionLoader.map[j][i] - 1]
										->AddTile(Tile(tileset.getImagePath(), position, textureRegion));
								}
								else // Exception for when the collision map decided to not work for some reason
								{
									objects.push_back(
										new CCollidableTile(
											mapTileSize.x * i + offset.x, 
											mapTileSize.y * j + offset.y, 
											textures->Get(STRING_TO_WSTRING(tileset.getImagePath())),
											imagePosition.x / mapTileSize.x, 
											imagePosition.y / mapTileSize.y, 
											tileSize.x, 
											tileSize.y)
									);
								}
							}
						}
					}
				}
			}
		}
		else if (layer->getType() == tmx::Layer::Type::Group) {
			const auto& layerGroup = layer->getLayerAs<tmx::LayerGroup>();
			LoadLayers(textures, tMap, layerGroup.getLayers(), tilesets, collisionLoader, collisionObjects, i);
		}
		i++;
	}
}

void CPlayScene::_ParseSection_PROPERTIES(string line) 
{
	std::vector<string> lines = split(line);
	if (lines.size() != 2) {
		DebugOut(L"[ERROR]: Unhandled parsing for PROPERTIES");
		return;
	}

	if (lines[0] == "map_path") {
		LoadMap(lines[1]);
	} 
}