#pragma once
#include "GameObject.h"
#include "TextGo.h"	
#include "SpriteGo.h"

class TextGo;
class SpriteGo;

class UiHUD : public GameObject
{
protected:
	UiHUD(const UiHUD&) = delete;
	UiHUD(UiHUD&&) = delete;
	UiHUD& operator=(const UiHUD&) = delete;
	UiHUD& operator=(UiHUD&&) = delete;
	
	std::string formatScore = "SCORE : ";
	std::string formatHighScore = "HIGH SCORE : ";
	std::string formatWave = "WAVE : ";
	std::string formatZombieCount = "ZOMBIES : ";
	
	sf::RectangleShape gaugeHp;
	sf::Vector2f referenceResolution = { 1920.f, 1080.f };
	sf::Vector2f resolution = referenceResolution;
	sf::Vector2f gaugeHpSize = { 400, 80 };

	TextGo textScore;
	TextGo textHighScore;
	TextGo textAmmo;
	TextGo textWave;
	TextGo textZombieCount;

	SpriteGo ammoIcon;

	int score;
	int highScore;
	int currentAmmo;
	int fullAmmo;
	int gunCapacity;
	int hp;
	int wave;
	int zombieCount;


public:
	UiHUD(const std::string& name = "");
	~UiHUD() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	//점수, 최고 점수, 총탄, 체력, 웨이브, 좀비 수

	int GetScore() const { return this->score; }
	void SetScore(int score);
	int GetHighScore() const { return this->highScore; }
	void SetHighScore(int score);
	void SetHp(int hp, int maxHp);
	int GetHp() const { return this->hp; }
	void SetWave(int wave);
	int GetZombieCount() const { return this->zombieCount; }
	void SetZombieCount(int zombieCount);
	void SetResolution(const sf::Vector2f& resolution);

	void SetAmmo(int current, int gunCap);
};