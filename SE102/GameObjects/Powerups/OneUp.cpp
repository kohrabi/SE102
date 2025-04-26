#include "OneUp.h"

#include "Engine/Loaders/SpritesLoader.h"
#include "GameObjects/ScorePopup.h"

#include "ContentIds/OneUp.h"

#include <iostream>
#include <algorithm>

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

void COneUp::OnDelete()
{
    CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, ScoreType::OneUp));
}

void COneUp::Render() {
    CSprites* const sprites = CSprites::GetInstance();

    sprites->Get(ONEUP_ID_SPRITE_IDLE)->Draw(position.x, position.y, GetLayer(layer, orderInLayer));
}