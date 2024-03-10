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

}

void Bullet::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));
	if(player->GetIsCheated()) SetTexture("graphics/Strikers1945/assets/bulletLevel4.bmp");
	else SetTexture("graphics/Strikers1945/assets/bulletLevel" + std::to_string(player->GetPowerLevel()) + ".bmp");
	SetOrigin(Origins::BC);

}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);
	time += dt;
	Translate(direction * speed * dt);

	if (abs(position.x) > 270 || abs(position.y) > 480)
	{
		SetActive(false);
		sceneGame->RemoveGameObject(this); // 이 부분은 오브젝트 풀링으로 변경 필요
	}
}

void Bullet::FixedUpdate(float dt)
{

	const std::list<Enemy*> list = sceneGame->enemyList;

	for (auto& enemy : list)
	{
		if (!enemy->GetActive() || enemy->isDead()) continue;

		if (GetGlobalBounds().intersects(enemy->GetGlobalBounds()))
		{
			enemy->OnDamage(player->GetDamage());
			SetActive(false);
			sceneGame->RemoveGameObject(this); // 이 부분은 오브젝트 풀링으로 변경 필요
		}
	}
}
