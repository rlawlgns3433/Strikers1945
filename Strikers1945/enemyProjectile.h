#pragma once
#include "SpriteGo.h"

class SceneGame;
class AnimPlayer;

class EnemyProjectile : public SpriteGo
{
protected:
	EnemyProjectile(const EnemyProjectile&) = delete;
	EnemyProjectile(EnemyProjectile&&) = delete;
	EnemyProjectile& operator=(const EnemyProjectile&) = delete;
	EnemyProjectile& operator=(EnemyProjectile&&) = delete;

	SceneGame* sceneGame = nullptr;
	AnimPlayer* player = nullptr;

	sf::Vector2f direction = { 0, 1 };

	float time = 0.f;
	float speed = 500.f;

public:
	EnemyProjectile(const std::string& name = "enemyProjectile");
	~EnemyProjectile() override = default;

	void Init() override;
	void Reset() override;;
	void Update(float dt) override;
	void FixedUpdate(float dt);

	void SetDirection(const sf::Vector2f direction);
};

