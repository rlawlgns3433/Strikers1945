#pragma once
#include "Scene.h"

class TextGo;
class SpriteGo;
class Button;

class SceneUpgrade : public Scene
{
protected:
	SceneUpgrade(const SceneUpgrade&) = delete;
	SceneUpgrade(SceneUpgrade&&) = delete;
	SceneUpgrade& operator=(const SceneUpgrade&) = delete;
	SceneUpgrade& operator=(SceneUpgrade&&) = delete;

	Button* powerUpButton = nullptr;
	Button* extraLifeButton = nullptr;
	Button* bombButton = nullptr;
	Button* startGameButton = nullptr;

	TextGo goldDisplay;
	SpriteGo* myFlight = nullptr;

	sf::Vector2f windowSize;
	sf::Sprite fighterSprite;
	sf::Font font;

	sf::Color buttonColor = sf::Color(127, 127, 127, 255);
	sf::Color buttonColorFocused = sf::Color(127, 127, 127, 200);
	sf::Color buttonColorPressed = sf::Color(127, 127, 127, 127);

	float alpha;

	long long currentGold = 0;
	int extraPowerLevel = 0;
	int extraLifes = 0;
	int extraBombs = 0;

	bool isButtonPressed = false;

public:
	SceneUpgrade(SceneIDs id);
	~SceneUpgrade() override = default;

	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	int GetGold();

	void UpgradePowerLevel();
	void UpgradeExtraLifes();
	void UpgradeExtraBombs();

	int GetExtraPowerLevel() const { return extraPowerLevel; }
	int GetExtraLifes() const { return extraLifes; }
	int GetExtraBombs() const { return extraBombs; }

	std::vector<int> GetExtraStat();
	void SaveGold();
};