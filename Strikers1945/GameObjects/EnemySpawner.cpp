#include "pch.h"
#include "EnemySpawner.h"

int EnemySpawner::enemyCount = 0;

EnemySpawner::EnemySpawner(const std::string& name)
	: Spawner(name)
{
}

GameObject* EnemySpawner::Create()
{
	Enemy::Types enemyType = enemyTypes[Utils::Random::RandomRange(0, enemyTypes.size() - 1)];
	Enemy* enemy = Enemy::Create(enemyType);


	return enemy;
}

void EnemySpawner::Init()
{
	Spawner::Init();
	interval = 3.f;
}

void EnemySpawner::Update(float dt)
{
	Spawner::Update(dt);

	if (background->GetPhase() == currentPhase /*&& currentPhase != Background::CommonEnemyPhase*/) return;

	switch (background->GetPhase())
	{
	case Background::CommonEnemyPhase:
		ResetCommonEnemyPhase();
		break;
	case Background::MidBossPhase:
		ResetMidBoosPhase();
		break;
	case Background::BossPhase:
		Enemy* enemy = Enemy::Create(Enemy::Types::Boss);
		break;
	}

	currentPhase = background->GetPhase();
	std::cout << currentPhase << std::endl;
	
}

void EnemySpawner::Release()
{
	Spawner::Release();
}

void EnemySpawner::Reset()
{
	Spawner::Reset();

	ResetCommonEnemyPhase();
}

void EnemySpawner::ResetCommonEnemyPhase()
{
	enemyTypes.clear();
	enemyTypes.push_back(Enemy::Types::MidBoss);

	//enemyTypes.push_back(Enemy::Types::Regular1);
	//enemyTypes.push_back(Enemy::Types::Regular1);
	//enemyTypes.push_back(Enemy::Types::Regular2);
	//enemyTypes.push_back(Enemy::Types::Regular2);
	//enemyTypes.push_back(Enemy::Types::Regular3);
	//enemyTypes.push_back(Enemy::Types::Regular3);
}

void EnemySpawner::ResetMidBoosPhase()
{
	ResetCommonEnemyPhase();
	enemyTypes.push_back(Enemy::Types::MidBoss);
}

void EnemySpawner::ResetBossPhase()
{
	enemyTypes.clear();
	enemyTypes.push_back(Enemy::Types::Boss);
}