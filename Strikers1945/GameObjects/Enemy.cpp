#include "pch.h"
#include "Enemy.h"
#include "EnemyProjectile.h"
#include "EnemyTable.h"
#include "Razer.h"
#include "SceneGame.h"
#include "AnimPlayer.h"
#include "Background.h"
#include "UiHUD.h"
#include "rapidcsv.h"

Enemy* Enemy::Create(Types enemyType)
{
	Enemy* enemy = new Enemy();
	enemy->type = enemyType;

	const EnemyData& data = ENEMY_TABLE->Get(enemyType);
	enemy->animationMoveClipId = data.animationMoveClipId;
	enemy->animationDeadClipId = data.animationDeadClipId;
	enemy->maxHp = data.maxHp;
	enemy->speed = data.speed;
	enemy->score = data.score;
	enemy->attackInterval = data.attackInterval;
	enemy->shootType = data.shootType;

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
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));
	hud = dynamic_cast<UiHUD*>(sceneGame->FindGameObject("hud"));
	background = dynamic_cast<Background*>(sceneGame->FindGameObject("background"));

	switch (type)
	{
	case Enemy::Types::Regular1:
	case Enemy::Types::Regular2:
	case Enemy::Types::Regular3:
	{
		std::function<void()> deadEvent = std::bind(&Enemy::DeadEvent, this);
		animator.AddEvent("animation/Enemy/Dead.csv", 9, deadEvent);
		regularEnemyMoveFuncs.push_back(std::bind(&Enemy::MoveStraight, this, std::placeholders::_1));
		regularEnemyMoveFuncs.push_back(std::bind(&Enemy::MoveOnCircle, this, std::placeholders::_1));
		regularEnemyMoveFuncs.push_back(std::bind(&Enemy::MoveSin, this, std::placeholders::_1));
		regularEnemyMoveFuncs.push_back(std::bind(&Enemy::MoveReturn, this, std::placeholders::_1));

		regularAttackFuncs.push_back(std::bind(&Enemy::ShootFrontOneTime, this));
		regularAttackFuncs.push_back(std::bind(&Enemy::ShootFrontThreeTime, this));
		regularAttackFuncs.push_back(std::bind(&Enemy::TargetingShotPattern, this, 1));
		storedFuncIdx = Utils::Random::RandomRange(0, regularEnemyMoveFuncs.size() - 1);
	}
		break;
	case Enemy::Types::MidBoss:
	{
		std::function<void()> midBossDeadEvent = std::bind(&Enemy::MidBossDeadEvent, this);
		animator.AddEvent("animation/Enemy/enemyMidBoss/Dead.csv", 14, midBossDeadEvent);
	}
		break;
	case Enemy::Types::Boss:
	{
		std::function<void()> bossDeadEvent = std::bind(&Enemy::BossDeadEvent, this);
		animator.AddEvent("animation/Enemy/enemyBoss/Dead.csv", 15, bossDeadEvent);
		bossMoveFuncs.push_back(std::bind(&Enemy::MoveRandom, this, std::placeholders::_1));
		bossMoveFuncs.push_back(std::bind(&Enemy::MoveTowardPlayer, this, std::placeholders::_1));
		bossAttackFuncs.push_back([this]() { this->ShootFrontThreeTime(); });
		bossAttackFuncs.push_back(std::bind(&Enemy::SpreadShotPattern, this, 10, 180, 300));
		bossAttackFuncs.push_back(std::bind(&Enemy::TargetingShotPattern, this, 1));

		bossRazer = new Razer();
		bossRazer->Init();
		bossRazer->Reset();
		bossRazer->SetActive(false);
		bossRazer->SetPosition(position);
		 
		sceneGame->AddGameObject(bossRazer);
	}
		break;
	case Enemy::Types::Speacial:
		break;
	case Enemy::Types::Gound:
		break;
	}
}

void Enemy::Reset()
{
	animator.Play(animationMoveClipId);
	SetOrigin(Origins::MC);
	Utils::Origin::SetOrigin(razerShape, Origins::TC);

	maxHp *= background->GetPhase();
	hp = maxHp;
}

void Enemy::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);
	continuousAttackTimer += dt;
	attackTimer += dt;

	if (!isAlive && animator.GetCurrentClipId() != animationDeadClipId)
	{
		animator.Play(animationDeadClipId);
	}

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

			if (hp < maxHp * 0.5f)
			{
				bossRazer->SetActive(true);
				RazerGunPattern(dt);
			}
		}
		else MoveStraight(dt);

		if (isAlive && bossRazer->GetGlobalBounds().intersects(player->GetGlobalBounds()) &&
			!player->GetIsInvincible())
		{
			player->OnDie();
		}
	}
	break;
	case Enemy::Types::Speacial:
		break;
	case Enemy::Types::Gound:
		break;
	}

	if (isAlive && GetGlobalBounds().intersects(player->GetGlobalBounds()) &&
		Utils::MyMath::Distance(player->GetPosition(), position) < 40 && 
		!player->GetIsInvincible())
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

void Enemy::Shoot()
{
	switch (shootType)
	{
	case Enemy::ShootTypes::Regular :
		if (player->GetIsBomb()) return;
		regularAttackFuncIdx = Utils::Random::RandomRange(0, regularAttackFuncs.size() - 1);
		regularAttackFuncs[regularAttackFuncIdx]();
		break;
	case Enemy::ShootTypes::MidBoss:
		if (player->GetIsBomb()) return;
		TargetingShotPattern();
		SpreadShotPattern(5, 180, 300);
		break;
	case Enemy::ShootTypes::Boss:
		bossAttackFuncIdx = Utils::Random::RandomRange(0, bossAttackFuncs.size() - 1);
		bossAttackFuncs[bossAttackFuncIdx]();
		break;
	}
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

		for (int i = 0; i < 3; ++i) 
		{
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
	bossRazer->SetPosition(position);
	bossRazer->SetOrigin(Origins::BC);
	sf::Transform tr;
	tr.rotate(60 * dt);
	razerDirection = tr * razerDirection;
	bossRazer->SetRotation(Utils::MyMath::Angle(razerDirection));
}

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

		do
		{
			speed = Utils::Random::RandomRange(150, 300);
			bossMovingDirection = Utils::MyMath::GetNormal(Utils::Random::GetRandomVector2(-1.f, 1.f));
		} while ((position.x < -FRAMEWORK.GetWindowSize().x * 0.5f && bossMovingDirection.x < 0) || (position.x > FRAMEWORK.GetWindowSize().x * 0.5f && bossMovingDirection.x > 0) ||
			(position.y < -FRAMEWORK.GetWindowSize().y * 0.5f && bossMovingDirection.y < 0));
	}

	sf::Vector2f newPos = position + bossMovingDirection * speed * dt;

	newPos.x = std::max(newPos.x, -FRAMEWORK.GetWindowSize().x * 0.5f);
	newPos.x = std::min(newPos.x, FRAMEWORK.GetWindowSize().x * 0.5f);
	newPos.y = std::max(newPos.y, -FRAMEWORK.GetWindowSize().y * 0.5f);
	newPos.y = std::min(newPos.y, FRAMEWORK.GetWindowSize().y * 0.5f);

	SetPosition(newPos);
}

void Enemy::MoveTowardPlayer(float dt)
{

	bossMovingChangeTimer += dt;

	if (bossMovingChangeTimer > bossMovingChangeInterval)
	{
		bossMovingChangeTimer = 0.f;
		isMoving = true;

		storedFuncIdx = Utils::Random::RandomRange(0, bossMoveFuncs.size() - 1);

		sf::Vector2f playerPosition = player->GetPosition();
		direction = Utils::MyMath::GetNormal(playerPosition - position);
		speed = Utils::Random::RandomRange(150.f, 250.f);
	}

	sf::Vector2f newPos = position + direction * speed * dt;

	newPos.x = std::max(newPos.x, -FRAMEWORK.GetWindowSize().x * 0.5f);
	newPos.x = std::min(newPos.x, FRAMEWORK.GetWindowSize().x * 0.5f);
	newPos.y = std::max(newPos.y, -FRAMEWORK.GetWindowSize().y * 0.5f);
	newPos.y = std::min(newPos.y, FRAMEWORK.GetWindowSize().y * 0.5f);

	SetPosition(newPos);
}