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
        item->SetTexture("graphics/Strikers1945/assets/Life.png");
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
}

void Item::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}
