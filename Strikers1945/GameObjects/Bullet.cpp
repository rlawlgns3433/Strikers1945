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
	SetTexture("graphics/Strikers1945/assets/bulletLevel" + std::to_string(player->GetPowerLevel()) + ".bmp");

	//switch (player->GetPowerLevel())
	//{
	//case 1 :
	//	break;
	//case 2 :
	//	SetTexture("graphics/Strikers1945/assets/bulletLevel2.bmp");
	//	break;
	//case 3 :
	//	SetTexture("graphics/Strikers1945/assets/bulletLevel3.bmp");
	//	break;
	//case 4 :
	//	SetTexture("graphics/Strikers1945/assets/bulletLevel4.bmp");
	//	break;
	////}
	//SetTexture("graphics/Strikers1945/projectile.png");
	//SetTextureRect({ 7, 20, 9, 16 });					 // Rect 정하기
	//SetScale({ 2.f, 2.f });
	SetOrigin(Origins::BC);

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
			enemy->OnDamage(player->GetDamage());
			SetActive(false);
			sceneGame->RemoveGameObject(this); // 이 부분은 오브젝트 풀링으로 변경 필요
		}
	}
}
