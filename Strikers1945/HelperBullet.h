#pragma once
#include "SpriteGo.h"
#include "Enemy.h"

class HelperBullet : public SpriteGo
{
protected :
	HelperBullet(const HelperBullet&)				= delete;
	HelperBullet(HelperBullet&&)					= delete;
	HelperBullet& operator=(const HelperBullet&)	= delete;
	HelperBullet& operator=(HelperBullet&&)			= delete;
	
	SceneGame* sceneGame = nullptr;
	Enemy* closestEnemy = nullptr;

	sf::Vector2f direction;

	float speed = 300.f;
	float damage = 50.f;

	bool isDeterminedTarget = false;

public:
	HelperBullet(const std::string& name = "helperBullet");
	~HelperBullet() override = default;

	void Init() override;
	void Reset() override;;
	void Update(float dt) override;
	void FixedUpdate(float dt);

	Enemy* FindClosestEnemy();
};

