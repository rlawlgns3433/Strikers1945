#pragma once
#include "SpriteGo.h"
#include "SceneGame.h"
#include "AnimPlayer.h"

class enemyProjectile : public SpriteGo
{
protected:
	enemyProjectile(const enemyProjectile&) = delete;
	enemyProjectile(enemyProjectile&&) = delete;
	enemyProjectile& operator=(const enemyProjectile&) = delete;
	enemyProjectile& operator=(enemyProjectile&&) = delete;

	SceneGame* sceneGame = nullptr;
	sf::Vector2f direction = { 0, -1 };
	AnimPlayer* player = nullptr;

	float time = 0.f;
	float speed = 500.f;

public:
	enemyProjectile(const std::string& name = "enemyProjectile");
	~enemyProjectile() override = default;

	void Init() override;
	void Reset() override;;
	void Update(float dt) override;;
};

