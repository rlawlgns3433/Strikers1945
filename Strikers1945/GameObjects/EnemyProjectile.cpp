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

	SetTexture("graphics/Strikers1945/assets/bullet.png");
	SetScale({ 0.5f, 0.5f });
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

	if (abs(position.y) > 500.f || abs(position.x) > 320.f)
	{
		SetActive(false);
		//sceneGame->RemoveGameObject(this);
	}
}

void EnemyProjectile::FixedUpdate(float dt)
{
	if (Utils::MyMath::Distance(player->GetPosition(), position) >= 25) return ;
	if (!player->IsDead() && GetGlobalBounds().intersects(player->GetGlobalBounds()))
	{
		player->OnDie();
		SetActive(false);
		//sceneGame->RemoveGameObject(this);
	}
}

void EnemyProjectile::SetDirection(const sf::Vector2f direction)
{
	this->direction = direction;
}