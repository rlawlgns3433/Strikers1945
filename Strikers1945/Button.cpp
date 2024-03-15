#include "pch.h"
#include "Button.h"
#include "TextGo.h"
#include "SceneUpgrade.h"

Button::Button(const std::string& name)
	: SpriteGo(name)
{
}

void Button::Init()
{	
	SpriteGo::Init();
}

void Button::Reset()
{
}

void Button::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Button::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(shape);
	window.draw(buttonText);
}

void Button::SetOrigin(Origins preset)
{
	if (preset == Origins::CUSTOM)
	{
		preset == Origins::TL;
	}

	originPreset = preset;
	Utils::Origin::SetOrigin(shape, originPreset);
}

void Button::SetOrigin(const sf::Vector2f& origin)
{
	originPreset = Origins::CUSTOM;
	this->origin = origin;
	shape.setOrigin(this->origin);
}

void Button::SetButton(sf::Vector2f size, sf::Vector2f position, sf::Color color, Origins origin)
{
	shape.setSize(size);
	shape.setPosition(position);
	shape.setFillColor(color);
	SetOrigin(origin);
}

void Button::SetButtonText(const sf::Font& font, const std::string& label, size_t labelSize, sf::Color labelColor, sf::Vector2f position, Origins origin)
{
	buttonText.setFont(font);
	buttonText.setString(label);
	buttonText.setCharacterSize(labelSize);
	buttonText.setFillColor(labelColor);
	buttonText.setPosition(position);
	Utils::Origin::SetOrigin(buttonText, origin);
}

sf::FloatRect Button::GetLocalBounds()
{
	return shape.getLocalBounds();
}

sf::FloatRect Button::GetGlobalBounds()
{
	return shape.getGlobalBounds();
}
