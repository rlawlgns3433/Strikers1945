#pragma once
#include "SpriteGo.h"
#include "Animator.h"
#include "SceneGame.h"
#include "AnimPlayer.h"

class Enemy : public SpriteGo
{
public :
	enum class Types
	{
		Regular1,
		Regular2,
		Regular3,
		MidBoss,
		Boss,
		Speacial,
		Gound
	};

	static const int totalTypes = 5;
	static Enemy* Create(Types zombieType);

protected :
	Enemy(const Enemy&)					= delete;
	Enemy(Enemy&&)						= delete;
	Enemy& operator=(const Enemy&)		= delete;
	Enemy& operator=(Enemy&&)			= delete;

	Types type;

	SceneGame* sceneGame = nullptr;
	AnimPlayer* player = nullptr;
	Animator animator;

	std::string animationClipId;

	sf::Vector2f direction = {0.f, 1.f};

	float speed = 300.f;
	float continuousAttackInterval = 1.f;
	float continuousAttackTimer = 0.f;
	int maxContinuousAttackCount = 5;
	int continuousAttackCount = maxContinuousAttackCount;

	float attackInterval = 3.f;
	float attackTimer = 0.f;


	int maxHp = 100;
	int hp = maxHp;
	int damage = 10;
	int projectileCount = 200;

	bool isAlive = true;

public :
	Enemy(const std::string& name = "enemy");
	~Enemy() override					= default;

	void Init();
	void Reset();
	void Update(float dt);
	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateGameover(float dt);
	void UpdatePause(float dt);
	void Draw(sf::RenderWindow& window);

	void Shoot();

	void OnDamage(float damge);
	void OnDie();

	bool isDead() const { return !isAlive; }
};