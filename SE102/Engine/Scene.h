#pragma once

#include <string>

using namespace std;

/*
*  Abstract class for a game scene
*/
class CScene
{
protected:
	int id;
	wstring sceneFilePath;

public: 
	CScene(int id, wstring filePath)
	{
		this->id = id;
		this->sceneFilePath = filePath;
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
};
typedef CScene * LPSCENE;