#pragma once

#include "GreenKoopa.h"
#include "Engine/CollisionCast.h"

class CRedKoopa : public CGreenKoopa {
private:
    static void LoadContent();
    static bool IsContentLoaded;
protected:

    CCollisionCast cast;

	int GetAnimationId() override;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
    CRedKoopa(float x, float y);


	void Update(float dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
};