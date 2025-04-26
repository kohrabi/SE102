#include "OneUp.h"

#include "Engine/Loaders/SpritesLoader.h"
#include "GameObjects/ScorePopup.h"

#include "ContentIds/OneUp.h"

#include <iostream>
#include <algorithm>
#include <Engine/PlayScene.h>

using namespace std;

bool COneUp::IsContentLoaded = false;

void COneUp::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(ONEUP_SPRITES_PATH);
}

void COneUp::Eat()
{
    CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, ScoreType::OneUp));
    CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (scene != NULL)
        scene->AddOneUp();
    Delete();
}

void COneUp::Render() {
    CSprites* const sprites = CSprites::GetInstance();

    sprites->Get(ONEUP_ID_SPRITE_IDLE)->Draw(position.x, position.y, GetLayer(layer, orderInLayer));
}