#pragma once
#include "Scene.h"

class Tilemap;
class UiHUD;
class AnimPlayer;
class EnemySpawner;
class Background;
class Enemy;
class EnemyProjectile;

class SceneGame : public Scene
{
protected :
	SceneGame(const SceneGame&)				= delete;
	SceneGame(SceneGame&&)					= delete;
	SceneGame& operator=(const SceneGame&)	= delete;
	SceneGame& operator=(SceneGame&&)		= delete;	
	
	GameStatus status = GameStatus::Game;

	EnemySpawner* enemySpawner = nullptr;
	Background* background = nullptr;


	float windowX = 0.f;
	float windowY = 0.f;

public :
	SceneGame(SceneIDs id);
	~SceneGame() override = default;

	Tilemap* tilemap = nullptr;
	UiHUD* hud = nullptr;
	AnimPlayer* player = nullptr;

	std::list<Enemy*> enemyList;
	std::vector<EnemyProjectile*> enemyProjectiles;

	void Init() override;
	void Release() override;
	virtual void Reset();
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateGameover(float dt);
	void UpdatePause(float dt);
	void Draw(sf::RenderWindow& window);

	GameStatus GetStatus() const { return this->status; }
	void SetStatus(GameStatus newStatus);

	bool IsInTilemap(const sf::Vector2f& point);
	sf::Vector2f ClampByTilemap(const sf::Vector2f& point);
	
};