#pragma once

#include <string>
#include <d3d10.h>
#include <d3dx10.h>

#include "GameObject.h"

using namespace std;

#define BACKGROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)

/*
*  Abstract class for a game scene
*/
class CScene
{
protected:
	int id;
	wstring sceneFilePath;
	D3DXCOLOR backgroundColor = BACKGROUND_COLOR;

	vector<LPGAMEOBJECT> objects;

	AABB levelBound{};
	LPGAMEOBJECT player;		
public: 
	CScene(int id, wstring filePath)
	{
		this->id = id;
		this->sceneFilePath = filePath;
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0; 

	const vector<LPGAMEOBJECT>& GetObjectList() const { return objects; }
	LPGAMEOBJECT const GetPlayer() const { return player; } 
	void AddObject(LPGAMEOBJECT object) { objects.push_back(object); }

	D3DXCOLOR GetBackgroundColor() { return backgroundColor; }
};
typedef CScene * LPSCENE;