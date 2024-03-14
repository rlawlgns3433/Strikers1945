#pragma once
#include "Scene.h"

class SpriteGo;
class SceneGame;
class AnimPlayer;
class TextGo;

class SceneEnding : public Scene
{
protected:
	SceneEnding(const SceneEnding&) = delete;
	SceneEnding(SceneEnding&&) = delete;
	SceneEnding& operator=(const SceneEnding&) = delete;
	SceneEnding& operator=(SceneEnding&&) = delete;

	std::vector<TextGo*> textRanking;
	std::vector<std::pair<std::string, int>> ranking;
	std::string saveName = "";
	sf::Vector2f windowSize;
	sf::Font font;
	sf::Clock clock;

	float nameInputInterval = 10.f;
	int playerScore;

public:
	SceneEnding(SceneIDs id);
	~SceneEnding() override = default;

	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateGameover(float dt);
	void UpdatePause(float dt);
	void SetStatus(GameStatus newStatus);

	std::vector<std::pair<std::string, int>>& GetScores();
	void SortRanking();
};

