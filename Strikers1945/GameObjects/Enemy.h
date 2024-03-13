#pragma once
#include "SpriteGo.h"
#include "Animator.h"
#include "SceneGame.h"
#include "AnimPlayer.h"
#include "UiHUD.h"
#include "Item.h"
#include "Background.h"
#include "Razer.h"

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
		MidBoss,
		Boss,
	};

	static const int totalTypes = 5;
	static Enemy* Create(Types enemyType);

protected :
	Enemy(const Enemy&)					= delete;
	Enemy(Enemy&&)						= delete;
	Enemy& operator=(const Enemy&)		= delete;
	Enemy& operator=(Enemy&&)			= delete;

	Types type;
	ShootTypes shootType;

	SceneGame* sceneGame = nullptr;
	AnimPlayer* player = nullptr;
	Background* background = nullptr;
	UiHUD* hud = nullptr;
	Item* item = nullptr;
	Item::Types itemType;
	Animator animator;

	std::vector<std::function<void(float)>> regularEnemyMoveFuncs;
	std::vector<std::function<void(float)>> bossMoveFuncs;

	std::vector<sf::Vector2f> midBossDirections;
	std::string animationClipId;

	sf::Clock clock;
	sf::Transform rot;
	sf::Transform rotatePatternAngle;

	sf::RectangleShape razerShape;
	sf::Vector2f razerDirection = { 1.f , 1.f };

	sf::Vector2f right = { 1.f, 0.f };
	sf::Vector2f direction = {0.f, 1.f};
	sf::Vector2f direction1 = {0,1};
	sf::Vector2f bossMovingDirection;
	sf::Vector2f rotateProjectileDirection = {0.f, 1.f};

	float rotateTimer = 0.f;
	float startAngle = -90.f;
	float endAngle = 270.f;
	float speed = 300.f;
	float continuousAttackInterval = 1.f;
	float continuousAttackTimer = 0.f;

	int maxContinuousAttackCount = 1;
	int continuousAttackCount = maxContinuousAttackCount;
	float attackInterval = 3.f;
	float attackTimer = 0.f;

	float bossMovingChangeTimer = 0.f;
	float bossMovingChangeInterval = 3;
	float rotatePatternTimer = 0.f;
	float rotatePatternInterval = 3.f;
	bool isRotatePattern = false;

	int storedFuncIdx;
	int maxHp = 100;
	int hp = maxHp;
	int damage = 10;
	int projectileCount = 200;
	int score = 0;

	bool isAlive = true;
	bool iscenter = true;

	bool isMoving = false;
	bool isPlaying = false;

	// 테스트중
	float shotTimer = 0.0f; // 타이머, 0.1초마다 리셋됩니다.
	float currentAngle = 0.0f; // 현재 발사 각도, 초기값은 0도 입니다.

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
	void SpreadShotPattern(int bulletsCount, float spreadAngle, float projectileSpeed);
	void TargetingShotPattern(int bulletsCount = 1);
	void RazerGunPattern(float dt);
	void RotateBossPattern(float dt);

	void MoveStraight(float dt);
	void MoveOnCircle(float dt);
	void MoveSin(float dt);
	void MoveReturn(float dt);
	void MoveRandom(float dt);
	void MoveTowardPlayer(float dt);

	void OnDamage(float damage);
	void OnDie();

	void SetDead(bool isDead) { this->isAlive = !isDead; }
	bool isDead() const { return !isAlive; }
	void DeadEvent();
	void MidBossDeadEvent();
	void BossDeadEvent();

};