#pragma once
#include "GameObject.h"
#include "TextGo.h"	
#include "SpriteGo.h"

class TextGo;
class SpriteGo;
class AnimPlayer;
class SceneGame;

class UiHUD : public GameObject
{
protected:
	UiHUD(const UiHUD&) = delete;
	UiHUD(UiHUD&&) = delete;
	UiHUD& operator=(const UiHUD&) = delete;
	UiHUD& operator=(UiHUD&&) = delete;
	
	// 나중에 이미지로 변경 필요
	std::string scoreFormat = "Score : ";
	std::string lifesFormat = "Lifes : ";
	std::string bombCountFormat = "Bombs : ";

	sf::Vector2f referenceResolution = { 960.f, 540.f };
	sf::Vector2f resolution = referenceResolution;
	sf::Vector2i windowSize = FRAMEWORK.GetWindowSize();

	SceneGame* sceneGame = nullptr;
	AnimPlayer* player = nullptr;
	TextGo textScore;
	TextGo textBombCount;
	TextGo textLifes;

	int score = 0;
	int bombCount = 0;
	int lifes = 1;

public:
	UiHUD(const std::string& name = "hud");
	~UiHUD() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	int GetScore() const { return score; }
	void AddScore(int add);
	void SetScore(int score);

	int GetLifes() const { return lifes; }
	void AddLifes(int add);
	void SetLifes(int lifes);

	int GetBombCount() const { return bombCount; }
	void AddBombCount(int add);
	void SetBombCount(int bombCount);

};