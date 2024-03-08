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
		enemy->attackInterval = 0.2f;
		enemy->damage = 10;
		enemy->speed = 300.f;
		enemy->velocity = { 100, 200 };
		enemy->animationClipId = "animation/Enemy/enemy1/Idle.csv";
		enemy->shootType = ShootTypes::OneTime;
		break;

	case Types::Regular2:
		enemy->maxHp = 100;
		enemy->attackInterval = 0.2f;
		enemy->damage = 10;
		enemy->speed = 300.f;
		enemy->velocity = { 100, 200 };
		enemy->animationClipId = "animation/Enemy/enemy2/Idle.csv";
		enemy->shootType = ShootTypes::ThreeTime;
		break;

	case Types::Regular3:
		enemy->maxHp = 100;
		enemy->attackInterval = 0.2f;
		enemy->damage = 10;
		enemy->speed = 300.f;
		enemy->velocity = { -100, 200 };
		enemy->animationClipId = "animation/Enemy/enemy3/Idle.csv";
		enemy->shootType = ShootTypes::ThreeTime;
		break;

	case Types::MidBoss:
		enemy->maxHp = 100;
		enemy->attackInterval = 0.1f;
		enemy->damage = 10;
		enemy->speed = 100.f;
		enemy->velocity = { -100, 200 };
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
	switch (shootType)
	{
	case Enemy::ShootTypes::OneTime:
		ShootFrontOneTime();
		break;
	case Enemy::ShootTypes::ThreeTime:
		ShootFrontThreeTime();
		break;
	default:
		break;
	}
}

void Enemy::UpdateAwake(float dt)
{
}

void Enemy::UpdateGame(float dt)
{
	animator.Update(dt);


	if (!isAlive && animator.GetCurrentClipId() != "animation/Enemy/Dead.csv")
	{
		animator.Play("animation/Enemy/Dead.csv");
	}
	continuousAttackTimer += dt;
	attackTimer += dt;

	// Enemy 이동 테스트 중

	switch (type)
	{
	case Enemy::Types::Regular1:
		MoveStraight(dt);
		break;
	case Enemy::Types::Regular2:
		MoveOnCircle(dt);
		break;
	case Enemy::Types::Regular3:
		MoveReturn(dt);
		break;
	case Enemy::Types::MidBoss:
		break;
	case Enemy::Types::Boss:
		break;
	case Enemy::Types::Speacial:
		break;
	case Enemy::Types::Gound:
		break;
	default:
		break;
	}


	// Enemy 이동 테스트 중

	// 플레이어와 충돌처리
	// 1. 충돌을 확인한다.
	if (isAlive && GetGlobalBounds().intersects(player->GetGlobalBounds()) &&
		Utils::MyMath::Distance(player->GetPosition(), position) < 40)
	{
		// 2. 플레이어에 데미지를 먹인다.
		player->OnDie();
		attackTimer = 0.f;
		// 3. Enemy를 지운다.
		animator.Play("animation/Enemy/Dead.csv");
	}

	if (isAlive && continuousAttackTimer > continuousAttackInterval)
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

	// 맵 아래로 충분히  나가게 된다면 오브젝트 삭제
	if (position.y > 500.f || position.x > 320.f || (direction.y < 0 && position.y < -550.f))
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
	isAlive = false;
	std::function<void()> deadEvent = std::bind(&Enemy::DeadEvent, this);
	animator.AddEvent("animation/Enemy/Dead.csv", 9, deadEvent);
}

void Enemy::ShootFrontOneTime()
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
	}
}

void Enemy::ShootFrontThreeTime()
{
	// TODO : 막 코딩 고치기... For문
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

		sf::Vector2f plus;
		sf::Vector2f minus;
		Utils::MyMath::AngleWithDirectionOffsets(Utils::MyMath::GetNormal(player->GetPosition() - position), plus, minus);

		EnemyProjectile* projectilePlus = new EnemyProjectile();
		projectilePlus->Init();
		projectilePlus->Reset();
		projectilePlus->SetPosition(position);
		projectilePlus->SetDirection(plus);
		sceneGame->AddGameObject(projectilePlus);
		sceneGame->enemyProjectiles.push_back(projectilePlus);

		EnemyProjectile* projectileMinus = new EnemyProjectile();
		projectileMinus->Init();
		projectileMinus->Reset();
		projectileMinus->SetPosition(position);
		projectileMinus->SetDirection(minus);
		sceneGame->AddGameObject(projectileMinus);
		sceneGame->enemyProjectiles.push_back(projectileMinus);
	}
}

void Enemy::DeadEvent()
{
	SetActive(false);
	sceneGame->RemoveGameObject(this);
	sceneGame->enemyList.remove(this);
}

void Enemy::MoveStraight(float dt)
{
	Translate(direction * speed * dt);
}

void Enemy::MoveOnCircle(float dt)
{
	if (position.y > -400.f && !isRotating && !isPlaying)
	{
		isRotating = true;
	}

	if (isRotating)
	{
		isPlaying = true;
		sf::Transform tr;
		tr.rotate(360 * dt);
		direction1 = tr * direction1;
		Translate((direction + direction1) * speed * dt);

		rotateTimer += dt;
		if (rotateTimer > 1.5f)
		{
			isRotating = false;
		}
	}
	else
	{
		Translate(direction * speed * dt);
	}
}


void Enemy::MoveSin(float dt)
{
	if (!isRotating && !isPlaying)
	{
		isRotating = true;
	}

	if (isRotating)
	{
		rotateTimer += dt;
		isPlaying = true;

		// direction1의 값은, -1 ~ 1
		// direction1은 timer시간 동안 0~360의 값이 나와야 한다.
		// direction1.x는 cos(0~360), y는 sin(0~360)

		float angle = ((rotateTimer / 10.f) * 360);
		float amplitude = 2.f;
		float period = 0.2f;

		direction1.x = sin(angle * period) * amplitude;

		// 이동 적용
		sf::Vector2f newVec = direction + direction1;
		Utils::MyMath::GetNormal(newVec);

		Translate(newVec * speed * dt);

		if (rotateTimer > 2.f)
		{
			isRotating = false;
		}
	}
	else
	{
		Translate(direction * speed * dt);
	}
}

// 내려갔다가 다시 올라가는 형태 
// 어떻게 파괴 처리할 지가 관건..
// 1. direction.y가 마이너스이고 position.y가 -550보다 작아지면 파괴
void Enemy::MoveReturn(float dt)
{
	if (position.y > 0 && !isRotating && !isPlaying)
	{
		isRotating = true;
	}

	if (isRotating)
	{
		rotateTimer += dt;
		isPlaying = true;

		sf::Transform tr;
		tr.rotate(360 * dt);
		direction1 = tr * direction1;
		Translate((direction + direction1) * speed * dt);

		std::cout << Utils::MyMath::Angle(direction + direction1) << std::endl;
		if (direction1.y >= -0.95f && rotateTimer > 0.5f)
		{
			isRotating = false;
			speed = -speed;
		}
	}
	else
	{
		Translate(direction * speed * dt);
	}
}
