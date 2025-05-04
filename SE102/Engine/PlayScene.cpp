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
#include "GameObjects/Blocks/QuestionBlock.h"
#include "GameObjects/Blocks/Brick.h"
#include "Engine/Helper.h"
#include "GameObjects/Blocks/OneWay.h"
#include "GameObjects/Particles/Coin.h"
#include "GameObjects/NPC/Goomba.h"
#include "GameObjects/NPC/Piranha.h"
#include "GameObjects/NPC/Koopa.h"

#include "GameObjects/TileObjects/CollidableTileLayer.h"
#include <GameObjects/Level/KillBarrier.h>
#include <GameObjects/Blocks/TeleportPipe.h>
#include <GameObjects/Blocks/LevelEnd.h>
#include <GameObjects/Camera.h>

using namespace std;

CPlayScene::CPlayScene(int id, wstring filePath):
	CScene(id, filePath)
{
	hud = NULL;
	levelTimer = LEVEL_TIME;
	stopTimer = false;
	outroLevelTimer = 0.0f;
	levelState = LevelState::Normal;
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_PROPERTIES 1

#define MAX_SCENE_LINE 1024

void CPlayScene::Load()
{
	hud = NULL;
	levelTimer = LEVEL_TIME;
	stopTimer = false;
	outroLevelTimer = 0.0f;
	levelState = LevelState::Normal;

	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath.c_str());

	levelState = LevelState::Normal;

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
	hud = new CHUD(this);
	camera = new CCamera();
	objects.push_back(camera);

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath.c_str());
}

void CPlayScene::Update(float dt)
{
	//cout << objects.size() << '\n';
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	CGame* const game = CGame::GetInstance();
	if (game->GetResetScene())
	{
		game->ResetScene();
	}
	if (!stopTimer)
		levelTimer -= dt;
	else if (levelState == LevelState::Outro)
	{
		if (outroLevelTimer > 0) outroLevelTimer -= dt;
		else
		{
			if (levelTimer > 0.0f)
			{
				outroLevelTimer = 1000.0f / 60.0f;
				cout << levelTimer << " " << 100.0f * min(round(levelTimer / 1000.0f), 20.0f) / 2.0f << '\n';
				if (levelTimer >= 20000.0f)
				{
					game->AddScore(100.0f * min(round(levelTimer / 1000.0f), 20.0f) / 2.0f);
					levelTimer = max(levelTimer - 20000.0f, 0.0f);
				}
				else if (levelTimer >= 11000.0f)
				{
					game->AddScore(100.0f * min(round(levelTimer / 1000.0f), 20.0f) / 2.0f);
					levelTimer = max(levelTimer - 11000.0f, 0.0f);
				}
				else
				{
					game->AddScore(100.0f * min(round(levelTimer / 1000.0f), 20.0f) / 2.0f);
					levelTimer = max(levelTimer - 2000.0f, 0.0f);
				}
				if (levelTimer == 0.0f)
					outroLevelTimer = 2000.0f;
			}
			else
				game->SetResetScene(true);
		}
	}

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT obj = objects[i];
		if (obj != NULL && obj->IsCollidable())
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	CTextures* const textures = CTextures::GetInstance();
	CGame* const game = CGame::GetInstance();
	float cx, cy;
	game->GetCamPos(cx, cy);

	cy += 70.0f / 2.0f;
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


	//game->DrawScreen(256.0f / 2.0f - 8.f, game->GetBackBufferHeight() - (60.0f - 48.0f), 0.0f, 1.0f, textures->Get(L"Content/menu.png"));
	hud->Render();
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

		const auto& mapBound = tMap.getBounds();
		levelBound.left = mapBound.left;
		levelBound.right = (mapBound.left + mapBound.width);
		levelBound.top = mapBound.top;
		levelBound.bottom = (mapBound.top + mapBound.height);

		AABB camBound = game->GetCameraBound();
		for (auto& bound : levelBounds)
		{
			if (CCollision::CheckAABBOverlaps(bound, camBound))
			{
				levelBound = bound;
				break;
			}
		}

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
			if (layer->getName() == "LevelBounds")
			{
				for (const auto& layerObject : layerObjects)
				{
					if (layerObject.getClass() == "CLevelBound")
					{
						const auto& aabb = layerObject.getAABB();
						AABB bound;
						bound.left = aabb.left;
						bound.right = (aabb.left + aabb.width);
						bound.top = aabb.top;
						bound.bottom = (aabb.top + aabb.height);
						levelBounds.push_back(bound);
					}
				}
				continue;
			}

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

				// Blocks
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
					if (layerObject.getName() == "QuestionBrick")
						questionBlock->SetBrick();
					objects.push_back(questionBlock);
				}
				else if (layerObject.getClass() == "COneWay") 
				{
					position = Vector2(aabb.left + aabb.width / 2.0f, aabb.top + aabb.height / 2.0f);
					COneWay* block = new COneWay(position.x, position.y, aabb.width, aabb.height);
					objects.push_back(block);
				}
				else if (layerObject.getClass() == "CTeleportPipe")
				{
					position = Vector2(aabb.left + aabb.width / 2.0f, aabb.top + aabb.height / 2.0f);

					Vector2 teleportTo = Vector2::Zero;
					int directionY = 0;
					for (const auto& property : layerObject.getProperties())
					{
						if (property.getName() == "teleportTo") {
							int objId = property.getObjectValue();
							if (objId == 0)
								continue;
							auto teleportToObject = find_if(layerObjects.begin(), layerObjects.end(), [&objId](const tmx::Object& other) { return objId == other.getUID(); });
							if (teleportToObject != layerObjects.end())
								teleportTo = Vector2(teleportToObject->getPosition().x, teleportToObject->getPosition().y);
						}
						else if (property.getName() == "directionY")
							directionY = property.getIntValue();
					}
					CTeleportPipe* block = new CTeleportPipe(position.x, position.y, aabb.width, aabb.height, teleportTo, directionY);
					objects.push_back(block);
				}
				else if (layerObject.getClass() == "CBrick")
				{
					//position = Vector2(aabb.left + aabb.width / 2.0f, aabb.top + aabb.height / 2.0f);
					CBrick* brick = new CBrick(position.x, position.y);
					objects.push_back(brick);
				}
				else if (layerObject.getClass() == "CLevelEnd")
				{
					//position = Vector2(aabb.left + aabb.width / 2.0f, aabb.top + aabb.height / 2.0f);
					CLevelEnd* obj = new CLevelEnd(position.x, position.y);
					objects.push_back(obj);
				}
				else if (layerObject.getClass() == "CCoin")
				{
					CCoin* coin = new CCoin(position.x, position.y);
					objects.push_back(coin);
				}
				else if (layerObject.getClass() == "CKillBarrier")
				{
					CKillBarrier* obj = new CKillBarrier(layerObject.getAABB().left, layerObject.getAABB().top, layerObject.getAABB().width, layerObject.getAABB().height);
					objects.push_back(obj);
				}
				
				// NPCs
				else if (layerObject.getClass() == "CGoomba")
				{
					CGoomba* obj = new CGoomba(position.x, position.y);
					if (layerObject.getName() == "RedGoomba")
						obj->SetRedGoomba();
					for (const auto& property : layerObject.getProperties())
					{
						if (property.getName() == "hasWing" && property.getBoolValue())
							obj->SetHasWing();
					}
					objects.push_back(obj);
				}
				else if (layerObject.getClass() == "CFirePiranha")
				{
					int height = -1;
					for (const auto& property : layerObject.getProperties())
					{
						if (property.getName() == "height") {
							height = property.getIntValue();
						}
					}
					ASSERT(height != -1, "height doesn't exists weird");
					CPiranha* obj = new CPiranha(position.x, position.y, height, layerObject.getName() == "GreenFirePiranha", true);
					objects.push_back(obj);
				}
				else if (layerObject.getClass() == "CPiranha")
				{
					int height = -1;
					for (const auto& property : layerObject.getProperties())
					{
						if (property.getName() == "height") {
							height = property.getIntValue();
						}
					}
					ASSERT(height != -1, "height doesn't exists weird");
					CPiranha* obj = new CPiranha(position.x, position.y, height, layerObject.getName() == "GreenPiranha", false);
					objects.push_back(obj);
				}
				else if (layerObject.getClass() == "CKoopa")
				{
					CKoopa* obj = new CKoopa(position.x, position.y);
					for (const auto& property : layerObject.getProperties())
					{
						if (property.getName() == "hasWing" && property.getBoolValue())
							obj->SetHasWing();
					}
					if (layerObject.getName() == "RedKoopa")
						obj->SetRedKoopa();
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
								if (collisionObjects.size() > 0 && collisionLoader.map[j][i] - 1 >= 0)
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