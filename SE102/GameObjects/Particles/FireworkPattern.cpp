#include "FireworkPattern.h"
#include "Firework.h"

#include <tmxlite/Map.hpp>

#include "Engine/Utils.h"
#include <Engine/debug.h>
#include <Engine/Game.h>
#include <Engine/PlayScene.h>
#include <Engine/Graphics/Animations.h>
#include <ContentIds/Firework.h>

void SpawnFireworkPattern(int pattern, Vector2 center)
{
	CGame* const game = CGame::GetInstance();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(game->GetCurrentScene());
	for (const auto& position : FireworkPatterns[pattern].Positions) {
		Vector2 spawnPos(
			position.x - 128.0 / 2.0f + center.x,
			position.y - 128.0 / 2.0f + center.y
			);
		playScene->AddObject(new CFirework(
			spawnPos.x,
			spawnPos.y,
			roundf(spawnPos.distance(center) * 5.0f)
			));
	}
	//CAnimation* animation = CAnimations::GetInstance()->Get(FIREWORK_ID_ANIMATION_NORMAL);
	//animation->Reset();
	//animation->SetLoop(false);
}

void LoadFireworkPattern(int pattern, string map)
{
	tmx::Map tMap;

	if (tMap.load(map)) {
		for (const auto& layer : tMap.getLayers()) {
			ASSERT(layer->getType() == tmx::Layer::Type::Object, "WTF")
				const auto& objectLayer = layer->getLayerAs<tmx::ObjectGroup>();
			const auto& layerObjects = objectLayer.getObjects();
			for (const auto& layerObject : layerObjects)
			{
				const auto& aabb = layerObject.getAABB();
				Vector2 position = Vector2::Zero;
				// Center position
				position = Vector2(aabb.left + aabb.width / 2.0f, aabb.top - aabb.height / 2.0f);
				FireworkPatterns[pattern].Positions.push_back(position);
			}
		}
	}
}


void LoadFireworkPatterns() {
	LoadFireworkPattern(0, "Content/Maps/fireworkMushroom.tmx");
	LoadFireworkPattern(1, "Content/Maps/fireworkFlower.tmx");
	LoadFireworkPattern(2, "Content/Maps/fireworkStar.tmx");
}