#include "pch.h"
#include "EnemyProjectile.h"
#include "SceneGame.h"
#include "AnimPlayer.h"

EnemyProjectile::EnemyProjectile(const std::string& name)
	: SpriteGo(name)
{
}

void EnemyProjectile::Init()
{
	SpriteGo::Init();
	SetTexture("graphics/Strikers1945/assets/bullet.bmp");
	SetTextureRect({ 7, 20, 9, 16 });					 // Rect ���ϱ�
	SetOrigin(Origins::BC);
}

void EnemyProjectile::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));
}

void EnemyProjectile::Update(float dt)
{
	SpriteGo::Update(dt);
	time += dt;
	Translate(direction * speed * dt);

	//if (GetGlobalBounds().intersects(player->GetGlobalBounds()))
	//{
	//	player->OnDie();
	//	SetActive(false);
	//	sceneGame->RemoveGameObject(this); // ������Ʈ Ǯ������ ���� �ʿ�
	//}
}

void EnemyProjectile::FixedUpdate(float dt)
{
	if (GetGlobalBounds().intersects(player->GetGlobalBounds()))
	{
		player->OnDie();
		SetActive(false);
		sceneGame->RemoveGameObject(this); // ������Ʈ Ǯ������ ���� �ʿ�
	}
}

void EnemyProjectile::SetDirection(const sf::Vector2f direction)
{
	this->direction = direction;
}