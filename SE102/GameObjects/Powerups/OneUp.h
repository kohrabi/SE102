#pragma once


#include "Mushroom.h"
/*

Bouncer_PUpVel:.byte $00, -$40, -$40, -$30, -$20, -$10, $00, $10, $20, $30, $40
*/

class COneUp : public CMushroom {
private:
    static void LoadContent();
    static bool IsContentLoaded;
public:
	// Tile number xTile counting from 0
	// Tile number yTile counting from 0
    COneUp(float x, float y) : CMushroom(x, y)
    { 
        LoadContent(); 
    }

    void Eat();

	void Render() override;
};