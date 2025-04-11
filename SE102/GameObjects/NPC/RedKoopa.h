#pragma once

#include "GreenKoopa.h"
#include "Engine/CollisionCast.h"

class CRedKoopa : public CGreenKoopa {
private:
    static void LoadContent();
    static bool IsContentLoaded;
protected:

    CCollisionCast cast;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
    CRedKoopa(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};