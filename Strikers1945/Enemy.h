#pragma once
#include "SpriteGo.h"
#include "Animator.h"
#include "SceneGame.h"
#include "AnimPlayer.h"

class Enemy : public SpriteGo
{
public :
	struct ClipInfo
	{
		std::string idle;
		std::string dead;
		bool isDead = false;

		ClipInfo()
		{
		}
		ClipInfo(const std::string& idle, const std::string& dead, bool isDead)
			: idle(idle), dead(dead), isDead(isDead)
		{
		}
	};

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

	enum class ShootTypes
	{
		OneTime,
		ThreeTime,

	};

	static const int totalTypes = 5;
	static Enemy* Create(Types zombieType);

protected :
	Enemy(const Enemy&)					= delete;
	Enemy(Enemy&&)						= delete;
	Enemy& operator=(const Enemy&)		= delete;
	Enemy& operator=(Enemy&&)			= delete;

	Types type;
	ShootTypes shootType;

	SceneGame* sceneGame = nullptr;
	AnimPlayer* player = nullptr;
	Animator animator;

	std::string animationClipId;

	sf::Transform rot;
	sf::Vector2f right = { 1.f, 0.f };

	sf::Vector2f direction = {0.f, 1.f};
	sf::Vector2f velocity;
	sf::Vector2f center;
	sf::Vector2f newVector;
	sf::Vector2f direction1 = {0,1};

	float rotateTimer = 0.f;
	float startAngle = -90.f;
	float endAngle = 270.f;

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
	bool iscenter = true;

	/// <summary>
	/// rotate Å×½ºÆ®
	/// </summary>
	bool isRotating = false;
	bool isPlaying = false;

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
	void ShootFrontOneTime();
	void ShootFrontThreeTime();

	void MoveStraight(float dt);
	void MoveOnCircle(float dt);
	void MoveSin(float dt);

	void OnDamage(float damge);
	void OnDie();

	void SetDead(bool isDead) { this->isAlive = !isDead; }
	bool isDead() const { return !isAlive; }
	void DeadEvent();
};