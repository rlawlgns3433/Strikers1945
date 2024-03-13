#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneGame;
class Tile;
class UiHUD;
class Bullet;
class PlayerHelper;

class AnimPlayer : public SpriteGo
{
public:
	struct ClipInfo
	{
		std::string idle;
		std::string move;
		std::string dead;
		bool isMove = false;
		bool isDead = false;

		ClipInfo()
		{
		}
		ClipInfo(const std::string& idle, const std::string& move, const std::string& dead, bool isMove, bool isDead)
			: idle(idle), move(move), dead(dead), isMove(isMove), isDead(isDead)
		{
		}
	};

protected:
	AnimPlayer(const AnimPlayer&) = delete;
	AnimPlayer(AnimPlayer&&) = delete;
	AnimPlayer& operator=(const AnimPlayer&) = delete;
	AnimPlayer& operator=(AnimPlayer&&) = delete;

	Animator animator;
	ClipInfo currentClipInfo;
	SceneGame* sceneGame = nullptr;
	UiHUD* hud = nullptr;

	std::vector<ClipInfo> clipInfos;
	std::vector<PlayerHelper*> playerHelpers;
	std::vector<sf::Vector2f> playerHelpersOffset;
	std::list<Bullet*> usingBulletlist;
	std::list<Bullet*> unusingBulletlist;

	sf::Vector2f velocity;
	sf::Vector2f direction;
	sf::RectangleShape shape;

	float speed = 500.f;
	float shootInterval = 0.08f;
	float shootTimer = 0.f;

	float invincibleTimer = 0.f;
	float invincibleInterval = 2.f;

	int maxHp = 200;
	int hp = maxHp;
	int score = 0;
	int lifes = 3;
	int bombCount = 2;
	int damage = 750;
	int currentHelperCount = 0;

	int maxPowerLevel = 4;
	int powerLevel = 1;

	bool isDead = false;
	bool isInvincible = false;
	bool isCheated = false;

public:
	AnimPlayer(const std::string& name = "player");
	~AnimPlayer() override = default;

	void Init() override;
	void Reset() override;
	void Release() override;
	void Update(float dt) override;
	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateGameover(float dt);
	void UpdatePause(float dt);
	void Draw(sf::RenderWindow& window) override;

	void Shoot();
	void OnDie();
	void DeadEvent();

	sf::Vector2f GetVelocity() const { return velocity; }
	void SetVelocity(const sf::Vector2f& velocity) { this->velocity = velocity; }

	inline bool IsDead() const { return isDead; }
	inline bool GetIsInvincible() const { return isInvincible; }
	inline void SetInvincible(bool invincible) { isInvincible = invincible; }

	inline int GetScore() const { return score; }
	inline void AddScore(int add) { score += add; };
	inline void SetScore(int score) { this->score = score; }

	inline int GetLife() const { return lifes; }
	inline void AddLife(int add) { lifes += add; };
	inline void SetLife(int life) { this->lifes = life; }

	inline int GetBombItem() const { return bombCount; }
	inline void AddBombItem(int add) { bombCount += add; };
	inline void SetBombItem(int bombCount) { this->bombCount = bombCount; }

	inline int GetDamage() const { return damage; }
	inline void AddDamage(int add) { damage += add; }
	inline void SetDamage(int damage) { this->damage = damage; }

	inline int GetPowerLevel() const { return powerLevel; }
	void AddPowerLevel(int add);
	inline void SetPowerLevel(int powerLevel);

	inline bool GetIsCheated() const { return isCheated; }
	void SetCheatMode();

	inline int GetHelperCount() const { return currentHelperCount; }
	inline void AddHelperCount(int add) { currentHelperCount += add; }
	inline void SetHelperCount(int currentHelperCount) { this->currentHelperCount = currentHelperCount; }
};