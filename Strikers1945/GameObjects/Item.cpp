#include "pch.h"
#include "Item.h"
#include "AnimPlayer.h"
#include "SceneGame.h"
#include "ItemTable.h"

Item* Item::Create(Types itemType)
{
	if (itemType == Types::None) return nullptr;

	Item* item = new Item();
	item->type = itemType;

	const ItemData& data = ITEM_TABLE->Get(itemType);
	item->animationClipId = data.animationClipId;
	item->movement = data.movement;
	item->speed = data.speed;

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
		Translate(sf::Vector2f(0, 1.f) * 100.f * dt);
		if (position.y > 500.f || position.x > 320.f || (speed < 0 && position.y < -550.f))
		{
			SetActive(false);
		}
		break;
	case Item::MovementType::Move:
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

	if (GetActive() && GetGlobalBounds().intersects(player->GetGlobalBounds()))
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
	}
}

void Item::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
