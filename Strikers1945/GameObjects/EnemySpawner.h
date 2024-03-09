#pragma once
#include "Spawner.h"
#include "Enemy.h"
#include "Background.h"

class EnemySpawner : public Spawner
{
protected :
	EnemySpawner(const EnemySpawner&)			   = delete;
	EnemySpawner(EnemySpawner&&)				   = delete;
	EnemySpawner& operator=(const EnemySpawner&) = delete;
	EnemySpawner& operator=(EnemySpawner&&)	   = delete;

	SceneGame* sceneGame = nullptr;

	std::vector<Enemy::Types> enemyTypes;
	Background::GamePhase currentPhase = Background::GamePhase::CommonEnemyPhase;

public :
	static int enemyCount;

	EnemySpawner(const std::string& name = "enemyspawner");
	~EnemySpawner() override					   = default;

	GameObject* Create() override;

	void Init() override;
	void Update(float dt) override;
	void Release() override;
	void Reset() override;

	void ResetCommonEnemyPhase();
	void ResetMidBoosPhase();
	void ResetBossPhase();
};