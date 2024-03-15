#pragma once
#include "SpriteGo.h"

class TextGo;
class SceneUpgrade;

class Button : public SpriteGo
{
protected:
	Button(const Button&) = delete;
	Button(Button&&) = delete;
	Button& operator=(const Button&) = delete;
	Button& operator=(Button&&) = delete;

	SceneUpgrade* sceneUpgrade = nullptr;
	sf::Text buttonText;
	sf::RectangleShape shape;

public:
	Button(const std::string& name = "button");
	~Button() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetOrigin(Origins preset)			  override;
	void SetOrigin(const sf::Vector2f& origin)override;

	void SetButton(sf::Vector2f size, sf::Vector2f position, sf::Color color, Origins origin);
	void SetButtonText(const sf::Font& font, const std::string& label, size_t labelSize, sf::Color labelColor, sf::Vector2f position, Origins origin);
	void SetButtonColor(sf::Color color);

	sf::FloatRect GetLocalBounds() override;
	sf::FloatRect GetGlobalBounds() override;
};

