#include "pch.h"
#include "Enemy.h"
#include "EnemyProjectile.h"

Enemy* Enemy::Create(Types enemyType)
{
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

	std::function<void()> deadEvent = std::bind(&Enemy::DeadEvent, this);
	animator.AddEvent("animation/Enemy/Dead.csv", 9, deadEvent);
	std::function<void()> midBossDeadEvent = std::bind(&Enemy::MidBossDeadEvent, this);
	animator.AddEvent("animation/Enemy/enemyMidBoss/Dead.csv", 14, midBossDeadEvent);
	std::function<void()> bossDeadEvent = std::bind(&Enemy::BossDeadEvent, this);
	animator.AddEvent("animation/Enemy/enemyBoss/Dead.csv", 15, bossDeadEvent);

	switch (type)
	{
	case Enemy::Types::Regular1:
	case Enemy::Types::Regular2:
	case Enemy::Types::Regular3:
		regularEnemyMoveFuncs.push_back(std::bind(&Enemy::MoveStraight, this, std::placeholders::_1));
		regularEnemyMoveFuncs.push_back(std::bind(&Enemy::MoveOnCircle, this, std::placeholders::_1));
		regularEnemyMoveFuncs.push_back(std::bind(&Enemy::MoveSin, this, std::placeholders::_1));
		regularEnemyMoveFuncs.push_back(std::bind(&Enemy::MoveReturn, this, std::placeholders::_1));
		storedFuncIdx = Utils::Random::RandomRange(0, regularEnemyMoveFuncs.size() - 1);
		break;
	case Enemy::Types::MidBoss:
		break;
	case Enemy::Types::Boss:
		bossMoveFuncs.push_back(std::bind(&Enemy::MoveRandom, this, std::placeholders::_1));
		bossMoveFuncs.push_back(std::bind(&Enemy::MoveTowardPlayer, this, std::placeholders::_1));
		break;
	case Enemy::Types::Speacial:
		break;
	case Enemy::Types::Gound:
		break;
	default:
		break;
	}
}

void Enemy::Reset()
{
	animator.Play(animationClipId);
	SetOrigin(Origins::MC);
	Utils::Origin::SetOrigin(razerShape, Origins::TC);

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
		TargetingShotPattern();
		break;
	case Enemy::ShootTypes::ThreeTime:
		ShootFrontThreeTime();
		break;
	case Enemy::ShootTypes::MidBoss:
		TargetingShotPattern();
		SpreadShotPattern(5, 180, 500);
	case Enemy::ShootTypes::Boss:


		//ShootFrontThreeTime();
		//SpreadShotPattern(15, 360, 1000);

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
	
	if (type == Enemy::Types::Boss)
	{
		if (!isAlive && animator.GetCurrentClipId() != "animation/Enemy/enemyBoss/Dead.csv")
		{
			animator.Play("animation/Enemy/enemyBoss/Dead.csv");
		}
	}
	else if (type == Enemy::Types::MidBoss)
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
	case Enemy::Types::Regular2:
	case Enemy::Types::Regular3:
	{
		regularEnemyMoveFuncs[storedFuncIdx](dt);
	}

		break;
	case Enemy::Types::MidBoss:
		MoveStraight(dt);
		break;
	case Enemy::Types::Boss:
	{
		if (position.y > -400.f)
		{
			bossMoveFuncs[storedFuncIdx](dt);
			rotatePatternTimer += dt;
			if (!isRotatePattern && rotatePatternTimer > rotatePatternInterval)
			{
				rotatePatternTimer = 0.f;
				isRotatePattern = true;
			}

			if (isRotatePattern)
			{
				RotateBossPattern(dt);
				if (rotatePatternTimer > rotatePatternInterval)
				{
					isRotatePattern = false;
					rotatePatternTimer = 0.f;
				}
			}

			//RotateBossPattern(dt);
			RazerGunPattern(dt);
		}
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

	// 플레이어 충돌
	if (isAlive && GetGlobalBounds().intersects(player->GetGlobalBounds()) &&
		Utils::MyMath::Distance(player->GetPosition(), position) < 40)
	{
		player->OnDie();
		attackTimer = 0.f;
	}

	if (isAlive && razerShape.getGlobalBounds().intersects(player->GetGlobalBounds()) && Utils::MyMath::Distance(player->GetPosition(), razerShape.getPosition()) < 40)
	{
		player->OnDie();
	}

	// 발사체 발사
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
		if (type == Enemy::Types::Boss) return;

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
	window.draw(razerShape);
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
		projectile->SetDirection(direction);
		sceneGame->usingProjectileList.push_back(projectile);
		sceneGame->AddGameObject(projectile);
	}
}

void Enemy::ShootFrontThreeTime()
{
	if (projectileCount >= 3)
	{
		attackTimer = 0.f;
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
}

void Enemy::MidBossDeadEvent()
{
	DeadEvent();
}

void Enemy::BossDeadEvent()
{
	DeadEvent();
	sceneGame->SetStatus(GameStatus::GameOver);
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

void Enemy::RazerGunPattern(float dt)
{
	razerShape.setFillColor(sf::Color::White);
	razerShape.setSize({ 30.f, 300.f });
	Utils::Origin::SetOrigin(razerShape, Origins::TC);
	razerShape.setPosition(position);

	sf::Transform tr;
	tr.rotate(60 * dt);
	razerDirection = tr * razerDirection;
	razerShape.setRotation(Utils::MyMath::Angle(razerDirection));
}

// 일정 시간(0.05초)마다 특정 각도로 발사
void Enemy::RotateBossPattern(float dt)
{
	if (clock.getElapsedTime().asSeconds() >= 0.03f)
	{
		rotatePatternAngle.rotate(360 * 0.03f);
		rotateProjectileDirection = rotatePatternAngle * rotateProjectileDirection;

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
		projectile->SetDirection(rotateProjectileDirection);
		sceneGame->usingProjectileList.push_back(projectile);
		sceneGame->AddGameObject(projectile);
		clock.restart();
	}
}

void Enemy::MoveStraight(float dt)
{
	Translate(sf::Vector2f(0, 1.f) * speed * dt);
}

void Enemy::MoveOnCircle(float dt)
{
	if (position.y > -400.f && !isMoving && !isPlaying)
	{
		isMoving = true;
	}

	if (isMoving)
	{
		isPlaying = true;
		sf::Transform tr;
		tr.rotate(360 * dt);
		direction1 = tr * direction1;
		Translate((direction + direction1) * speed * 0.5f * dt);

		rotateTimer += dt;
		if (rotateTimer > 3.f)
		{
			isMoving = false;
		}
	}
	else
	{
		Translate(direction * speed * dt);
	}
}


void Enemy::MoveSin(float dt)
{
	if (!isMoving && !isPlaying)
	{
		isMoving = true;
	}

	if (isMoving)
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

		Translate(newVec * speed * 0.5f * dt);

		if (rotateTimer > 4.f)
		{
			isMoving = false;
		}
	}
	else
	{
		Translate(direction * speed * dt);
	}
}

void Enemy::MoveReturn(float dt)
{
	if (position.y > 0 && !isMoving && !isPlaying)
	{
		isMoving = true;
	}

	if (isMoving)
	{
		rotateTimer += dt;
		isPlaying = true;

		sf::Transform tr;
		tr.rotate(360 * dt);
		direction1 = tr * direction1;
		Translate(Utils::MyMath::GetNormal(direction + direction1) * speed * 0.5f * dt);

		if (direction1.y >= -0.95f && rotateTimer > 4.f)
		{
			isMoving = false;
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
		storedFuncIdx = Utils::Random::RandomRange(0, bossMoveFuncs.size() - 1);
		speed = Utils::Random::RandomRange(150, 300);
		bossMovingDirection = Utils::MyMath::GetNormal(Utils::Random::GetRandomVector2(-1.f, 1.f));
		if ((position.x < -FRAMEWORK.GetWindowSize().x * 0.5f && bossMovingDirection.x < 0)|| (position.x > FRAMEWORK.GetWindowSize().x * 0.5f && bossMovingDirection.x > 0) ||
			(position.y < -FRAMEWORK.GetWindowSize().y * 0.5f && bossMovingDirection.y < 0)) return;
	}

	Translate(bossMovingDirection * speed * dt);
}

void Enemy::MoveTowardPlayer(float dt)
{
	bossMovingChangeTimer += dt;

	if ((position.x < -FRAMEWORK.GetWindowSize().x * 0.5f && bossMovingDirection.x < 0) || (position.x > FRAMEWORK.GetWindowSize().x * 0.5f && bossMovingDirection.x > 0) ||
		(position.y < -FRAMEWORK.GetWindowSize().y * 0.5f && bossMovingDirection.y < 0)) return;

	// bossMovingChangeInterval이 경과했을 때만 새로운 방향과 속도를 설정합니다.
	if (bossMovingChangeTimer > bossMovingChangeInterval)
	{
		bossMovingChangeTimer = 0.f; // 타이머 리셋
		isMoving = true; // 움직임 시작

		// 이동 함수 인덱스를 랜덤하게 선택합니다.
		storedFuncIdx = Utils::Random::RandomRange(0, bossMoveFuncs.size() - 1);

		// 플레이어의 위치를 기반으로 새 방향과 속도를 설정합니다.
		sf::Vector2f playerPosition = player->GetPosition();
		direction = Utils::MyMath::GetNormal(playerPosition - position);
		speed = Utils::Random::RandomRange(150.f, 250.f);
	}

	// isMoving이 true일 때만 이동을 수행합니다.
	if (isMoving)
	{
		Translate(direction * speed * dt);

	}

}