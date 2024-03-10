#include "pch.h"
#include "Item.h"
#include "AnimPlayer.h"
#include "SceneGame.h"  

Item* Item::Create(Types itemType)
{
    // TODO
    // 아이템 등장에 대한 고민 필요

    Item* item = new Item();
    item->type = itemType;

    switch (item->type)
    {
    case Types::Life:
        item->animationClipId = "animation/Item/Life.csv";
        item->movement = MovementType::Move;
        break;
    case Types::Gold:
        item->animationClipId = "animation/Item/Gold.csv";
        item->movement = MovementType::NotMove;
        item->speed = 0.f;
        break;
    case Types::PowerUp:
        item->animationClipId = "animation/Item/PowerUp.csv";
        item->movement = MovementType::Move;
        break;
    case Types::Bomb:
        item->animationClipId = "animation/Item/Bomb.csv";
        item->movement = MovementType::Move;
        break;
    default :
        return nullptr;
    }

    return item;
}

Item::Item(const std::string& name)
    : SpriteGo(name)
{
}

void Item::Init()
{
    SpriteGo::Init();
    animator.SetTarget(&sprite);
    sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
    player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));
    hud = dynamic_cast<UiHUD*>(sceneGame->FindGameObject("hud"));
    direction = Utils::Random::GetRandomVector2(-1.f, 1.f);
}

void Item::Reset()
{
    animator.Play(animationClipId);
    SetOrigin(Origins::MC);
}

void Item::Update(float dt)
{
    SpriteGo::Update(dt);
    animator.Update(dt);

    switch (movement)
    {
    case Item::MovementType::NotMove:
        //위치를 고정
        Translate(sf::Vector2f(0, 1.f) * 100.f * dt);
        // 맵 밖으로 나가면 소멸
        if (position.y > 500.f || position.x > 320.f || (speed < 0 && position.y < -550.f))
        {
            SetActive(false);
            sceneGame->RemoveGameObject(this);
        }
        break;
    case Item::MovementType::Move:
        // 이동 로직을 구현
        // 네 면에 닿게 되면 새로운 벡터로 변경
        if (abs(position.x) >= 260 || abs(position.y) >= 480)
        {
            direction = Utils::Random::GetRandomVector2(-1.f, 1.f);
        }

        Translate(direction * speed * dt);
        break;
    }

}

void Item::LateUpdate(float dt)
{
    SpriteGo::LateUpdate(dt);
}

void Item::FixedUpdate(float dt)
{
    SpriteGo::FixedUpdate(dt);

    if (GetGlobalBounds().intersects(player->GetGlobalBounds()))
    {
        switch (type)
        {
        case Item::Types::Life:
            player->AddLife(1);
            hud->SetLifes(player->GetLife());
            break;
        case Item::Types::Gold:
            player->AddScore(100);
            hud->SetScore(player->GetScore());
            break;
        case Item::Types::PowerUp:
            player->AddPowerLevel(1);
            break;
        case Item::Types::Bomb:
            player->AddBombItem(1);
            hud->SetBombCount(player->GetBombItem());
            break;
        }

        SetActive(false);
        sceneGame->RemoveGameObject(this);
    }

}

void Item::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}
