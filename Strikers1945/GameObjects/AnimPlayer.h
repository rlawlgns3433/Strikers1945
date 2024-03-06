#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneGame;
class Tile;

class AnimPlayer : public SpriteGo
{
public:
	struct ClipInfo
	{
		std::string idle;
		std::string move;
		bool isMove = false;

		ClipInfo()
		{
		}
		ClipInfo(const std::string& idle, const std::string& move, bool isMove)
			: idle(idle), move(move), isMove(isMove)
		{
		}
	};

protected:
	AnimPlayer(const AnimPlayer&) = delete;
	AnimPlayer(AnimPlayer&&) = delete;
	AnimPlayer& operator=(const AnimPlayer&) = delete;
	AnimPlayer& operator=(AnimPlayer&&) = delete;

	Animator animator;
	SceneGame* sceneGame = nullptr;
	ClipInfo currentClipInfo;

	std::vector<ClipInfo> clipInfos;

	sf::Vector2f velocity;
	sf::Vector2f direction;

	float speed = 500.f;
	float shootInterval = 0.1f;
	float shootTimer = 0.f;

	int maxHp = 200;
	int hp = maxHp;

	bool isDead = false;

public:
	AnimPlayer(const std::string& name = "player");
	~AnimPlayer() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateGameover(float dt);
	void UpdatePause(float dt);
	void Draw(sf::RenderWindow& window) override;

	sf::Vector2f GetVelocity() const { return velocity; }
	void SetVelocity(const sf::Vector2f& velocity) { this->velocity = velocity; }

	void Shoot();
	void OnDie();
};