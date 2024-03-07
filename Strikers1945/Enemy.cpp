#include "pch.h"
#include "Enemy.h"
#include "EnemyProjectile.h"

Enemy* Enemy::Create(Types zombieType)
{
	Enemy* enemy = new Enemy();
	enemy->type = zombieType;
	
	switch (enemy->type)
	{
	case Types::Regular1 :
		enemy->maxHp = 100;
		enemy->attackInterval = 0.1f;
		enemy->damage = 10;
		enemy->speed = 100.f;
		enemy->animationClipId = "animation/Enemy/enemy1/Idle.csv";
		break;

	case Types::Regular2:
		enemy->maxHp = 100;
		enemy->attackInterval = 0.1f;
		enemy->damage = 10;
		enemy->speed = 100.f;
		enemy->animationClipId = "animation/Enemy/enemy2/Idle.csv";
		break;

	case Types::Regular3:
		enemy->maxHp = 100;
		enemy->attackInterval = 0.1f;
		enemy->damage = 10;
		enemy->speed = 100.f;
		enemy->animationClipId = "animation/Enemy/enemy3/Idle.csv";
		break;

	case Types::MidBoss:
		enemy->maxHp = 100;
		enemy->attackInterval = 0.1f;
		enemy->damage = 10;
		enemy->speed = 100.f;
		enemy->animationClipId = "animation/Enemy/enemy3/Idle.csv";
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
	if (projectileCount > 0)
	{
		attackTimer = 0.f;
		--projectileCount;
		EnemyProjectile* projectile = new EnemyProjectile();
		projectile->Init();
		projectile->Reset();
		projectile->SetPosition(position);
		projectile->SetDirection(Utils::MyMath::GetNormal(player->GetPosition() - position));
		sceneGame->AddGameObject(projectile);
		sceneGame->enemyProjectiles.push_back(projectile);
		std::cout << "enemy shoot" << std::endl;
	}
}

void Enemy::UpdateAwake(float dt)
{
}

void Enemy::UpdateGame(float dt)
{
	animator.Update(dt);
	continuousAttackTimer += dt;
	attackTimer += dt;

	Translate(direction * speed * dt);

	// �÷��̾�� �浹ó��
	// 1. �浹�� Ȯ���Ѵ�.
	if (GetGlobalBounds().intersects(player->GetGlobalBounds()) &&
		Utils::MyMath::Distance(player->GetPosition(), position) < 40)
	{
		// 2. �÷��̾ �������� ���δ�.
		player->OnDie();
		attackTimer = 0.f;
		// 3. Enemy�� �����.
		OnDie();
	}

	if (continuousAttackTimer > continuousAttackInterval)
	{
		if (continuousAttackCount > 0)
		{
			if (attackTimer > attackInterval)
			{
				attackTimer = 0.f;
				--continuousAttackCount;
				Shoot();
			}
		}
		else
		{
			continuousAttackTimer = 0.f;
			continuousAttackCount = maxContinuousAttackCount;
		}
	}

	// �� �Ʒ��� �����  ������ �ȴٸ� ������Ʈ ����
	
	if (position.y > 500.f)
	{
		OnDie();
	}
	else if (position.y > 100.f)
	{
		continuousAttackCount = 0;
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
	sceneGame->enemyList.remove(this);
}