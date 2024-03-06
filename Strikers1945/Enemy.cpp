#include "pch.h"
#include "Enemy.h"

Enemy* Enemy::Create(Types zombieType)
{
	Enemy* enemy = new Enemy();
	enemy->type = zombieType;
	
	switch (enemy->type)
	{
	case Types::Regular1 :
		enemy->maxHp = 100;
		enemy->attackInterval = 5.f;
		enemy->damage = 10;
		enemy->speed = 100.f;
		enemy->animationClipId = "animation/Enemy/Idle.csv";
		break;

	case Types::Regular2:
		enemy->maxHp = 100;
		enemy->attackInterval = 5.f;
		enemy->damage = 10;
		enemy->speed = 100.f;
		break;

	case Types::MidBoss:
		enemy->maxHp = 100;
		enemy->attackInterval = 5.f;
		enemy->damage = 10;
		enemy->speed = 100.f;
		break;

	case Types::Boss:

		break;
	case Types::Speacial:

		break;
	case Types::Gound:

		break;

	default:
		break;
	}

	//const DataZombie& data = ZOMBIE_TABLE->Get(zombieType);
	//zombie->textureId = data.textureId;
	//zombie->maxHp = data.maxHp;
	//zombie->speed = data.speed;
	//zombie->damage = data.damage;
	//zombie->attackInterval = data.attackInterval;

	enemy->SetPosition({ Utils::Random::RandomRange(-270, 270), 550.f });

    return enemy;
}
Enemy::Enemy(const std::string& name)
    : SpriteGo(name)
{
}
void Enemy::Init()
{
    SpriteGo::Init();
	animator.SetTarget(&sprite);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));
}

void Enemy::Reset()
{
	animator.Play(animationClipId);
	SetOrigin(Origins::MC);
}

void Enemy::Update(float dt)
{
	SpriteGo::Update(dt);

	switch (sceneGame->GetStatus())
	{
	case GameStatus::Awake:
		UpdateAwake(dt);
		break;
	case GameStatus::Game:
		UpdateGame(dt);
		break;
	case GameStatus::GameOver:
		UpdateGameover(dt);
		break;
	case GameStatus::Pause:
		UpdatePause(dt);
		break;
	}
}

void Enemy::Shoot()
{
}

void Enemy::UpdateAwake(float dt)
{
}

void Enemy::UpdateGame(float dt)
{
	animator.Update(dt);
	Translate(direction * speed * dt);

	attackTimer += dt;
	// 플레이어와 충돌처리
	// 1. 충돌을 확인한다.


	if (GetGlobalBounds().intersects(player->GetGlobalBounds()) &&
		Utils::MyMath::Distance(player->GetPosition(), position) < 40)
	{
		// 2. 플레이어에 데미지를 먹인다.
		player->OnDie();

		attackTimer = 0.f;
		// 3. Enemy를 지운다.
		OnDie();
	}
}

void Enemy::UpdateGameover(float dt)
{
}

void Enemy::UpdatePause(float dt)
{
}

void Enemy::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Enemy::OnDamage(float damge)
{
	hp -= damage;
	if (hp <= 0)
	{
		hp = 0;
		OnDie();
	}
}

void Enemy::OnDie()
{
	SetActive(false);
	sceneGame->RemoveGameObject(this);
}
