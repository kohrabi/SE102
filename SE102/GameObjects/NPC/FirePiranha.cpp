#include "FirePiranha.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/FirePiranha.h"

#include "Engine/Helper.h"

#include "Fireball.h"
#include "GreenKoopa.h"

#include <iostream>
#include <algorithm>
#include <GameObjects/ScorePopup.h>

using namespace std;

bool CFirePiranha::IsContentLoaded = false;

void CFirePiranha::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(FIRE_PIRANHA_SPRITES_PATH);
}

void CFirePiranha::OnDelete()
{
    CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, Score100));
}

void CFirePiranha::SetState(int state)
{
    switch (state)
    {
    case FIRE_PIRANHA_DOWN:
    {
        timer = FIRE_PIRANHA_DOWN_TIME;
        velocity.y = 0;
    }
    break;
    case FIRE_PIRANHA_MOVE:
    {
        if (position.y <= spawnPosition.y)
        {
            timer = FIRE_PIRANHA_MOVE_TIME;
            velocity.y = FIRE_PIRANHA_Y_SPEED;
        }
        else
        {
            timer = FIRE_PIRANHA_MOVE_TIME;
            velocity.y = -FIRE_PIRANHA_Y_SPEED;
        }
    }
    break;
    case FIRE_PIRANHA_UP:
    {
        timer = FIRE_PIRANHA_DOWN_TIME;
        velocity.y = 0;
    }
    break;
    case FIRE_PIRANHA_WAIT:
    {
        if (position.y <= spawnPosition.y)
        {
            shootTimer = FIRE_PIRANHA_WAIT_SHOOT_TIMER;
            canShoot = true;
        }
        timer = FIRE_PIRANHA_UP_TIME;
        velocity.y = 0;
    }
    break;
    }
    this->state = state;
}

void CFirePiranha::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CGreenKoopa*>(e->obj))
    {
        CGreenKoopa* koopa = dynamic_cast<CGreenKoopa*>(e->obj);
        if (koopa->IsInShell())
            this->Delete();
    }
}

void CFirePiranha::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
{
    if (!IsColliderInCamera())
        return;

    CGame* const game = CGame::GetInstance();
    LPGAMEOBJECT player = game->GetCurrentScene()->GetPlayer();

    // Look direction
    if (player != NULL)
    {
        lookDir.x = position.x < player->GetPosition().x ? 1 : -1;
        lookDir.y = position.y < player->GetPosition().y ? 1 : -1;
    }

    // Shoot fireball
    if (canShoot)
    {
        if (shootTimer > 0) shootTimer -= dt;
        else
        {
            game->GetCurrentScene()->AddObject(new CFireball(position.x + lookDir.x * 2.0f, position.y - 4.0f, lookDir.x, lookDir.y));
            canShoot = false;
            shootTimer = FIRE_PIRANHA_WAIT_SHOOT_TIMER;
            shootAnimation = FIRE_PIRANHA_SHOOT_ANIMATION_TIME;
        }
        //if (shootAnimation <= 0.0f && shootTimer <= FIRE_PIRANHA_WAIT_SHOOT_TIMER / 2.0f)
    }
    if (shootAnimation > 0) shootAnimation -= dt;


    if (timer > 0) timer -= dt;
    else
    {
        if (state == FIRE_PIRANHA_DOWN || state == FIRE_PIRANHA_UP) SetState(FIRE_PIRANHA_MOVE);
        else if (state == FIRE_PIRANHA_MOVE) SetState(FIRE_PIRANHA_WAIT);
        else if (state == FIRE_PIRANHA_WAIT) SetState(FIRE_PIRANHA_MOVE);
    }
    position.y = clampf(position.y + velocity.y * dt, spawnPosition.y, spawnPosition.y + FIRE_PIRANHA_DOWN_POS);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFirePiranha::Render() {
    CSprites* const sprites = CSprites::GetInstance();
    CGame* const game = CGame::GetInstance();
    LPGAMEOBJECT object = game->GetCurrentScene()->GetPlayer();
    
    if (object != NULL)
    {
        Vector2 playerPosition = object->GetPosition() - position;
        bool flipX = false;
        int animationID = FIRE_PIRANHA_ID_SPRITE_LOOK_DOWN_CLOSE;
        if (shootAnimation <= 0)
        {
            if (lookDir.x > 0)
                flipX = true;
            if (lookDir.y < 0)
                animationID = FIRE_PIRANHA_ID_SPRITE_LOOK_UP_CLOSE;
        }
        else
        {
            animationID = FIRE_PIRANHA_ID_SPRITE_LOOK_DOWN_OPEN;
            if (lookDir.x > 0)
                flipX = true;
            if (lookDir.y < 0)
                animationID = FIRE_PIRANHA_ID_SPRITE_LOOK_UP_OPEN;

        }
        sprites->Get(animationID)->Draw(position.x, position.y, GetLayer(layer, orderInLayer), flipX);
    }
}