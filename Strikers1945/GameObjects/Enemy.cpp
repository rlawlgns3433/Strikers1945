#include "pch.h"
#include "Enemy.h"
#include "EnemyProjectile.h"

Enemy* Enemy::Create(Types enemyType)
{
	// TODO
	// background의 phase 마다 등장하는 enemy의 종류가 다양해짐
	// boss 페이즈 진행 시 더 이상 다른 몬스터를 생성하지 않고 보스만 하나 생성

	Enemy* enemy = new Enemy();
	enemy->type = enemyType;
	
	switch (enemy->type)
	{
	case Types::Regular1 :
		enemy->maxHp = 200;
		enemy->attackInterval = 0.5f;
		enemy->speed = 150.f;
		enemy->score = 100;
		enemy->animationClipId = "animation/Enemy/enemy1/Idle.csv";
		enemy->shootType = ShootTypes::OneTime;
		break;

	case Types::Regular2:
		enemy->maxHp = 300;
		enemy->attackInterval = 0.5f;
		enemy->speed = 170.f;
		enemy->score = 200;
		enemy->animationClipId = "animation/Enemy/enemy2/Idle.csv";
		enemy->shootType = ShootTypes::OneTime;
		break;

	case Types::Regular3:
		enemy->maxHp = 300;
		enemy->attackInterval = 0.5f;
		enemy->speed = 180.f;
		enemy->score = 300;
		enemy->animationClipId = "animation/Enemy/enemy3/Idle.csv";
		enemy->shootType = ShootTypes::OneTime;
		break;

	case Types::MidBoss:
		enemy->maxHp = 3000;
		enemy->attackInterval = 0.3f;
		enemy->speed = 50.f;
		enemy->score = 500;
		enemy->animationClipId = "animation/Enemy/enemyMidBoss/Idle.csv";
		enemy->shootType = ShootTypes::MidBoss;
		break;

	case Types::Boss:
		enemy->maxHp = 6000;
		enemy->attackInterval = 0.1f;
		enemy->speed = 50.f;
		enemy->score = 500;
		enemy->animationClipId = "animation/Enemy/enemyBoss/Idle.csv";
		enemy->shootType = ShootTypes::Boss; 
		break;
	case Types::Speacial:

		break;
	case Types::Gound:

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
	hud = dynamic_cast<UiHUD*>(sceneGame->FindGameObject("hud"));
	background = dynamic_cast<Background*>(sceneGame->FindGameObject("background"));
}

void Enemy::Reset()
{
	animator.Play(animationClipId);
	SetOrigin(Origins::MC);

	maxHp *= (background->GetPhase() + 1);
	hp = maxHp;
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
		TargetingShotPattern(1);
		break;
	case Enemy::ShootTypes::ThreeTime:
		ShootFrontThreeTime();
		break;
	case Enemy::ShootTypes::MidBoss:
		TargetingShotPattern(1);
		SpreadShotPattern(5, 180, 500);
	case Enemy::ShootTypes::Boss:
		ShootFrontThreeTime();
		SpreadShotPattern(15, 360, 1000);

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
	if (type == Enemy::Types::MidBoss)
	{
		if (!isAlive && animator.GetCurrentClipId() != "animation/Enemy/enemyMidBoss/Dead.csv")
		{
			animator.Play("animation/Enemy/enemyMidBoss/Dead.csv");
		}
	}
	else
	{
		if (!isAlive && animator.GetCurrentClipId() != "animation/Enemy/Dead.csv")
		{
			animator.Play("animation/Enemy/Dead.csv");
		}
	}

	continuousAttackTimer += dt;
	attackTimer += dt;

	// Enemy 이동 테스트 중

	switch (type)
	{
	case Enemy::Types::Regular1:
		(this->*funcPointers[3])(dt);
	case Enemy::Types::Regular2:
		(this->*funcPointers[1])(dt);
	case Enemy::Types::Regular3:
		(this->*funcPointers[2])(dt);
		break;
	case Enemy::Types::MidBoss:
		MoveStraight(dt);
		break;
	case Enemy::Types::Boss:
	{
		if (position.y > -400.f) MoveRandom(dt);
		else MoveStraight(dt);
	}
		break;
	case Enemy::Types::Speacial:
		break;
	case Enemy::Types::Gound:
		break;
	default:
		break;
	}

	if (isAlive && GetGlobalBounds().intersects(player->GetGlobalBounds()) &&
		Utils::MyMath::Distance(player->GetPosition(), position) < 40)
	{
		player->OnDie();
		attackTimer = 0.f;
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
	if (position.y > 500.f || (speed < 50 && position.y < -600.f))
	{
		DeadEvent();
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

void Enemy::OnDamage(float damage)
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
	if (isAlive)
	{
		player->AddScore(score);
		hud->SetScore(player->GetScore());

		int itemChance = (int)(Utils::Random::RandomValue() * 100);
		
		if (itemChance < 1) itemType = Item::Types::Life;
		else if (itemChance < 41) itemType = Item::Types::Gold;
		else if (itemChance < 46) itemType = Item::Types::PowerUp;
		else if (itemChance < 50) itemType = Item::Types::Bomb;
		else itemType = Item::Types::None;

		item = Item::Create(itemType);
		if (item != nullptr)
		{
			item->Init();
			item->Reset();
			item->SetPosition(position);
			sceneGame->ItemList.push_back(item);
			sceneGame->AddGameObject(item);
		}
	}

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

		EnemyProjectile* projectile = nullptr;
		if (sceneGame->unusingProjectileList.empty())
		{
			projectile = new EnemyProjectile();
			projectile->Init();
		}
		else
		{
			projectile = sceneGame->unusingProjectileList.front();
			sceneGame->unusingProjectileList.pop_front();
		}
		projectile->Reset();
		projectile->SetActive(true);
		projectile->SetPosition(position);
		projectile->SetDirection(Utils::MyMath::GetNormal(player->GetPosition() - position));
		sceneGame->usingProjectileList.push_back(projectile);
		sceneGame->AddGameObject(projectile);
	}
}

void Enemy::ShootFrontThreeTime()
{
	if (projectileCount >= 3)
	{
		attackTimer = 0.f;
		direction = Utils::MyMath::GetNormal(player->GetPosition() - position);
		sf::Vector2f directions[3];
		Utils::MyMath::AngleWithDirectionOffsets(direction, directions[1], directions[2]);
		directions[0] = direction;

		for (int i = 0; i < 3; ++i) {
			--projectileCount;

			EnemyProjectile* projectile = nullptr;
			if (sceneGame->unusingProjectileList.empty()) 
			{
				projectile = new EnemyProjectile();
				projectile->Init();
			}
			else 
			{
				projectile = sceneGame->unusingProjectileList.front();
				sceneGame->unusingProjectileList.pop_front();
			}

			projectile->Reset();
			projectile->SetActive(true);
			projectile->SetPosition(position);
			projectile->SetDirection(directions[i]);
			sceneGame->usingProjectileList.push_back(projectile);
			sceneGame->AddGameObject(projectile);
		}
	}
}


void Enemy::DeadEvent()
{
	SetActive(false);
	sceneGame->enemyList.remove(this);
	sceneGame->RemoveGameObject(this);

	if (type == Enemy::Types::Boss)
	{
		sceneGame->SetStatus(GameStatus::GameOver);
	}
}

void Enemy::BossPattern()
{
}

void Enemy::SpreadShotPattern(int bulletsCount, float spreadAngle, float projectileSpeed)
{
	sf::Vector2f directionToPlayer = Utils::MyMath::GetNormal(player->GetPosition() - position);
	float playerAngle = std::atan2(directionToPlayer.y, directionToPlayer.x);
	float angleBetweenBullets = (spreadAngle * (3.14159265f / 180.0f)) / (bulletsCount - 1);
	float startingAngle = playerAngle - (spreadAngle * (3.14159265f / 180.0f) / 2);
	
	for (int i = 0; i < bulletsCount; ++i)
	{
		float bulletAngle = startingAngle + angleBetweenBullets * i;
		sf::Vector2f bulletDirection = sf::Vector2f(std::cos(bulletAngle), std::sin(bulletAngle));

		EnemyProjectile* projectile = nullptr;
		if (sceneGame->unusingProjectileList.empty())
		{
			projectile = new EnemyProjectile();
			projectile->Init();
		}
		else
		{
			projectile = sceneGame->unusingProjectileList.front();
			sceneGame->unusingProjectileList.pop_front();
		}

		projectile->Reset();
		projectile->SetActive(true);
		projectile->SetPosition(position);
		projectile->SetSpeed(projectileSpeed);
		projectile->SetDirection(bulletDirection);
		sceneGame->usingProjectileList.push_back(projectile);
		sceneGame->AddGameObject(projectile);
	}

}

void Enemy::TargetingShotPattern(int bulletsCount)
{
	sf::Vector2f playerPosition = player->GetPosition();

	for (int i = 0; i < bulletsCount; ++i)
	{
		EnemyProjectile* projectile = nullptr;
		if (sceneGame->unusingProjectileList.empty())
		{
			projectile = new EnemyProjectile();
			projectile->Init();
		}
		else
		{
			projectile = sceneGame->unusingProjectileList.front();
			sceneGame->unusingProjectileList.pop_front();
		}

		projectile->Reset();
		projectile->SetActive(true);
		projectile->SetPosition(position);
		projectile->SetDirection(Utils::MyMath::GetNormal(playerPosition - position));
		sceneGame->usingProjectileList.push_back(projectile);
		sceneGame->AddGameObject(projectile);
	}
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
		Translate(Utils::MyMath::GetNormal(direction + direction1) * speed * dt);

		if (direction1.y >= -0.95f && rotateTimer > 1.5f)
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

void Enemy::MoveRandom(float dt)
{
	bossMovingChangeTimer += dt;
	if (bossMovingChangeTimer > bossMovingChangeInterval || abs(position.x) > 200 || position.y > 0)
	{
		bossMovingChangeTimer = 0.f;
		speed = Utils::Random::RandomRange(50, 300);
		bossMovingDirection = { Utils::Random::GetRandomVector2(-1.f, 1.f)};
		//Utils::MyMath::Normalize(bossMovingDirection);
	}

	Translate(bossMovingDirection * speed * dt);
}
