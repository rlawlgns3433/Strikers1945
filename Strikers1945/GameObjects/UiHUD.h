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
	

	sf::Vector2f referenceResolution = { 960.f, 540.f };
	sf::Vector2f resolution = referenceResolution;

	TextGo textScore;
	TextGo textBombCount;

	int score;
	int bombCount;
	int lifes;

public:
	UiHUD(const std::string& name = "");
	~UiHUD() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	int GetScore() const { return score; }
	void SetScore(int score);

	int GetLifes() const { return lifes; }
	void AddLifes(int add);
	void SetLifes(int lifes);

	int GetBombCount() const { return bombCount; }
	void AddBombCount(int add);
	void SetBombCount(int bombCount);

};