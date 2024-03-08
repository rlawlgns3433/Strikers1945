#include "pch.h"
#include "Bullet.h"
#include "Enemy.h"

Bullet::Bullet(const std::string& name)
	: SpriteGo(name)
{
}

void Bullet::Init()
{
	SpriteGo::Init();
	SetTexture("graphics/Strikers1945/projectile.png");
	SetTextureRect({7, 20, 9, 16});					 // Rect ���ϱ�
	SetScale({ 2.f, 2.f });
	SetOrigin(Origins::BC);
}

void Bullet::Reset()
{
	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));
	damage = 200;
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);
	time += dt;
	Translate(direction * speed * dt);
}

void Bullet::FixedUpdate(float dt)
{

	const std::list<Enemy*> list = sceneGame->enemyList;

	for (auto& enemy : list)
	{
		if (!enemy->GetActive() || enemy->isDead()) continue;

		if (GetGlobalBounds().intersects(enemy->GetGlobalBounds()))
		{
			enemy->OnDamage(damage);
			SetActive(false);
			sceneGame->RemoveGameObject(this); // �� �κ��� ������Ʈ Ǯ������ ���� �ʿ�
		}
	}
}