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

	sf::Vector2f windowSize;
	sf::Sprite fighterSprite;
	sf::Font font;

	sf::Color buttonColor = sf::Color(127, 127, 127, 255);
	sf::Color buttonColorFocused = sf::Color(127, 127, 127, 200);
	sf::Color buttonColorPressed = sf::Color(127, 127, 127, 127);

	float alpha;

	size_t currentGold = 0;
	int extraPowerLevel = 0;
	int extraLifes = 0;
	int extraBombs = 0;

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

	size_t GetCurrentGold() const { return currentGold; }
	void AddCurrentGold(size_t add) { currentGold += add; }
	void SetCurrentGold(size_t currentGold) { this->currentGold = currentGold; }

	int GetExtraPowerLevel() const { return extraPowerLevel; }
	void AddExtraPowerLevel(int add) { extraPowerLevel += add; }
	void SetExtraPowerLevel(int extraPowerLevel) { this->extraPowerLevel = extraPowerLevel; }

	int GetExtraLifes() const { return extraLifes; }
	void AddExtraLifes(int add) { extraLifes += add; }
	void SetExtraLifes(int extraLifes) { this->extraLifes = extraLifes; }

	int GetExtraBombs() const { return extraBombs; }
	void AddExtraBombs(int add) { extraBombs += add; }
	void SetExtraBombs(int extraBombs) { this->extraBombs = extraBombs; }

	std::vector<int> GetExtraStat();
};