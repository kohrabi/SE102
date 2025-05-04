#include "Piranha.h"

#include "Engine/Loaders/SpritesLoader.h"

#include "ContentIds/FirePiranha.h"
#include "ContentIds/GreenFirePiranha.h"
#include "ContentIds/GreenPiranha.h"

#include "Engine/Helper.h"

#include "Fireball.h"
#include "Koopa.h"

#include <iostream>
#include <algorithm>
#include <GameObjects/Particles/ScorePopup.h>

using namespace std;

bool CPiranha::IsContentLoaded = false;

void CPiranha::LoadContent() 
{
    if (IsContentLoaded)
        return;
    IsContentLoaded = true;
    SpritesLoader loader;
    loader.Load(FIRE_PIRANHA_SPRITES_PATH);
    loader.Load(GREEN_FIRE_PIRANHA_SPRITES_PATH);
    loader.Load(GREEN_PIRANHA_SPRITES_PATH);
}

void CPiranha::OnDelete()
{
    CGame::GetInstance()->GetCurrentScene()->AddObject(new CScorePopup(position.x, position.y, ScoreCombo));
}

void CPiranha::SetState(int state)
{
    switch (state)
    {
    case FIRE_PIRANHA_DOWN:
    {
        timer = FIRE_PIRANHA_DOWN_TIME * heightCount / 2.0f;
        velocity.y = 0;
    }
    break;
    case FIRE_PIRANHA_MOVE:
    {
        if (isUp)
        {
            timer = FIRE_PIRANHA_MOVE_TIME * heightCount / 2.0f;
            velocity.y = FIRE_PIRANHA_Y_SPEED;
        }
        else
        {
            timer = FIRE_PIRANHA_MOVE_TIME * heightCount / 2.0f;
            velocity.y = -FIRE_PIRANHA_Y_SPEED;
        }
        isUp = !isUp;
    }
    break;
    case FIRE_PIRANHA_UP:
    {
        timer = FIRE_PIRANHA_DOWN_TIME * heightCount / 2.0f;
        velocity.y = 0;
    }
    break;
    case FIRE_PIRANHA_WAIT:
    {
        if (isUp && isFirePiranha)
        {
            shootTimer = FIRE_PIRANHA_WAIT_SHOOT_TIMER * heightCount / 2.0f;
            canShoot = true;
        }
        timer = FIRE_PIRANHA_UP_TIME * heightCount / 2.0f;
        velocity.y = 0;
    }
    break;
    }
    this->state = state;
}

void CPiranha::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CKoopa*>(e->obj))
    {
        CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
        if (koopa->IsInShell())
            this->Delete();
    }
}

void CPiranha::Update(float dt, vector<LPGAMEOBJECT> *coObjects)
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
            shootTimer = FIRE_PIRANHA_WAIT_SHOOT_TIMER * heightCount / 2.0f;
            shootAnimation = FIRE_PIRANHA_SHOOT_ANIMATION_TIME * heightCount / 2.0f;
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
    if (heightCount == 2)
        position.y = clampf(position.y + velocity.y * dt, spawnPosition.y, spawnPosition.y + FIRE_PIRANHA_DOWN_POS);
    else
        position.y = clampf(position.y + velocity.y * dt, spawnPosition.y + 8.0f, spawnPosition.y + FIRE_PIRANHA_DOWN_POS);

    CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CPiranha::GetFireAnimationId()
{
    if (!isGreen)
    {
        if (shootAnimation <= 0)
        {
            if (lookDir.y < 0)
                return FIRE_PIRANHA_ID_SPRITE_LOOK_UP_CLOSE;
            return FIRE_PIRANHA_ID_SPRITE_LOOK_DOWN_CLOSE;
        }
        else
        {
            if (lookDir.y < 0)
                return FIRE_PIRANHA_ID_SPRITE_LOOK_UP_OPEN;
            return FIRE_PIRANHA_ID_SPRITE_LOOK_DOWN_OPEN;
        }
    }
    else
    {
        if (shootAnimation <= 0)
        {
            if (lookDir.y < 0)
                return GREEN_FIRE_PIRANHA_ID_SPRITE_LOOK_UP_CLOSE;
            return GREEN_FIRE_PIRANHA_ID_SPRITE_LOOK_DOWN_CLOSE;
        }
        else
        {
            if (lookDir.y < 0)
                return GREEN_FIRE_PIRANHA_ID_SPRITE_LOOK_UP_OPEN;
            return GREEN_FIRE_PIRANHA_ID_SPRITE_LOOK_DOWN_OPEN;
        }

    }
}

void CPiranha::Render() {
    CGame* const game = CGame::GetInstance();
    LPGAMEOBJECT object = game->GetCurrentScene()->GetPlayer();
    
    if (object != NULL)
    {
        bool flipX = lookDir.x > 0;
        if (isFirePiranha)
        {
            int animationID = GetFireAnimationId();
            CSprites::GetInstance()->Get(animationID)->Draw(position.x, position.y, GetLayer(layer, orderInLayer), flipX);
        }
        else
        {
            CAnimations::GetInstance()->Get(GREEN_PIRANHA_ID_ANIMATION_NORMAL)->Render(position.x, position.y, GetLayer(layer, orderInLayer), flipX);
        }
    }
}