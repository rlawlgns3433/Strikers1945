#include "pch.h"
#include "Item.h"
#include "AnimPlayer.h"
#include "SceneGame.h"  

Item* Item::Create(Types itemType)
{
    Item* item = new Item();
    item->type = itemType;

    switch (item->type)
    {
    case Types::Life :
        item->SetTexture("graphics/Strikers1945/assets/Life.png");
        item->movement = MovementType::Move;
        break;
    case Types::Gold :
        item->animationClipId = "animation/Item/Gold.csv";
        item->movement = MovementType::NotMove;
        break;
    case Types::PowerUp :
        item->animationClipId = "animation/Item/PowerUp.csv";
        item->movement = MovementType::Move;
        break;
    case Types::Bomb :
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
