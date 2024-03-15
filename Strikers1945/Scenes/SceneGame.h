#pragma once
#include "Scene.h"

class Item;
class Tilemap;
class UiHUD;
class AnimPlayer;
class EnemySpawner;
class Background;
class Enemy;
class EnemyProjectile;
class SpriteGo;
class TextGo;
class SceneUpgrade;

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
	SpriteGo* pauseWindow = nullptr;
	TextGo* textCountDown = nullptr;
	TextGo* saveName = nullptr;

	SceneUpgrade* sceneUpgrade = nullptr;

	std::vector<std::pair<std::string, int>> ranking;
	std::string saveNameFormat = "Save Name : ";

	sf::RectangleShape fadeWindow;
	sf::Clock clock;
	sf::Font font;

	int countDown = 10;
	int hiScore;
	int gold;

	float nameInputInterval = 10.f;
	float countDownTimer = 0.f;
	float countDownInterval = 1.f;
	float windowX = 0.f;
	float windowY = 0.f;
	float alpha;

	bool isNewRecord;
	bool isCoinInserted = false;

public :
	SceneGame(SceneIDs id);
	~SceneGame() override = default;

	UiHUD* hud = nullptr;
	AnimPlayer* player = nullptr;

	std::list<Enemy*> enemyList;
	std::list<EnemyProjectile*> usingProjectileList;
	std::list<EnemyProjectile*> unusingProjectileList;
	std::list<Item*> ItemList;

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

	void GetReward();

	std::vector<std::pair<std::string, int>>& GetScores();
	int GetGold();
	int GetHighScore();
	void SaveHighScore();
	void SortRanking();

	const std::list<Enemy*>& GetEnemyList() const { return enemyList; }
	const std::list<EnemyProjectile*>& GetUsingProjectileList() const { return usingProjectileList; }
	std::list<EnemyProjectile*>& GetUnusingProjectileList() { return unusingProjectileList; }
};