#include "pch.h"
#include "PlayerHelper.h"
#include "AnimPlayer.h"

PlayerHelper::PlayerHelper(const std::string& name)
	: SpriteGo(name)
{
}

void PlayerHelper::Init()
{
	SpriteGo::Init();
}

void PlayerHelper::Reset()
{
	for (auto& bullet : usingHelperBulletlist)
	{
		bullet->SetActive(false);
	}

	for (auto& bullet : unusingHelperBulletlist)
	{
		bullet->SetActive(false);
	}

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));

	SetTexture("graphics/Strikers1945/playerHelper.png");
	SetOrigin(Origins::MC);
}

void PlayerHelper::Update(float dt)
{
	SpriteGo::Update(dt);
	SetPosition(player->GetPosition() + offset);

	attackTimer += dt;

	Shoot();

	// 재사용 구간
	auto it = usingHelperBulletlist.begin();
	while (it != usingHelperBulletlist.end())
	{
		auto bullet = *it;
		if (!bullet->GetActive())
		{
			it = usingHelperBulletlist.erase(it);
			unusingHelperBulletlist.push_back(bullet);
		}
		else
		{
			++it;
		}
	}
}

void PlayerHelper::Shoot()
{

	if (attackTimer > attackInterval)
	{
		attackTimer = 0.f;

		HelperBullet* bullet = nullptr;
		if (unusingHelperBulletlist.empty())
		{
			bullet = new HelperBullet();
			bullet->Init();
		}
		else
		{
			bullet = unusingHelperBulletlist.front();
			unusingHelperBulletlist.pop_front();
		}

		bullet->SetActive(true);
		bullet->Reset();
		bullet->SetPosition(position);

		usingHelperBulletlist.push_back(bullet);
		sceneGame->AddGameObject(bullet);
	}
}

void PlayerHelper::SetOffset(const sf::Vector2f& offset)
{
	this->offset = offset;
	SetPosition(player->GetPosition() + offset);
}
