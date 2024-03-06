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
	interval = 5.f;
}

void EnemySpawner::Release()
{
	Spawner::Release();
}

void EnemySpawner::Reset()
{
	Spawner::Reset();

	enemyTypes.clear();
	enemyTypes.push_back(Enemy::Types::Regular1);
	enemyTypes.push_back(Enemy::Types::Regular1);
	enemyTypes.push_back(Enemy::Types::Regular1);
	enemyTypes.push_back(Enemy::Types::Regular1);
	enemyTypes.push_back(Enemy::Types::Regular1);
	enemyTypes.push_back(Enemy::Types::Regular1);
}
