#include "pch.h"
#include "UiHUD.h"

UiHUD::UiHUD(const std::string& name)
	: GameObject(name)
{
}

void UiHUD::Init()
{
	GameObject::Init();
	textScore.Init();
	textHighScore.Init();
	textAmmo.Init();
	ammoIcon.Init();
	textWave.Init();
	textZombieCount.Init();

	float textSize = 60.f;
	sf::Font& font = *FONT_MANAGER.GetResource("fonts/zombiecontrol.ttf");

	textScore.Set(font, "", textSize, sf::Color::White);
	textHighScore.Set(font, "", textSize, sf::Color::White);
	textAmmo.Set(font, "", textSize, sf::Color::White);
	textWave.Set(font, "", textSize, sf::Color::White);
	textZombieCount.Set(font, "", textSize, sf::Color::White);

	ammoIcon.SetTexture("graphics/ammo_icon.png");
	gaugeHp.setFillColor(sf::Color::Red);
	gaugeHp.setSize(gaugeHpSize);

	textScore.SetOrigin(Origins::TL);
	textHighScore.SetOrigin(Origins::TR);
	ammoIcon.SetOrigin(Origins::BL);
	textAmmo.SetOrigin(Origins::BL);
	Utils::Origin::SetOrigin(gaugeHp, Origins::BL);
	textWave.SetOrigin(Origins::BR);
	textZombieCount.SetOrigin(Origins::BR);

	float topY = 50.f;

	textScore.SetPosition({ 150.f, topY });
	textHighScore.SetPosition({ referenceResolution.x - 50, topY });

	float bottomY = referenceResolution.y - 50;
	ammoIcon.SetPosition({ 50.f, bottomY });
	textAmmo.SetPosition({ 100.f, bottomY });
	gaugeHp.setPosition({ 400.f, bottomY });

	textWave.SetPosition({ referenceResolution.x - 600.f, bottomY });
	textZombieCount.SetPosition({ referenceResolution.x - 150.f, bottomY });
}

void UiHUD::Reset()
{
	GameObject::Reset();

	textScore.SetText(formatScore);
	textHighScore.SetText(formatHighScore);
	textAmmo.SetText("6/6");
	textWave.SetText(formatWave);
	textZombieCount.SetText(formatZombieCount);
	gaugeHp.setSize(gaugeHpSize);
}

void UiHUD::Update(float dt)
{
	GameObject::Update(dt);

	textScore.Update(dt);
	textHighScore.Update(dt);
	textAmmo.Update(dt);
	textWave.Update(dt);
	textZombieCount.Update(dt);
	ammoIcon.Update(dt);
}

void UiHUD::LateUpdate(float dt)
{
	GameObject::LateUpdate(dt);
}

void UiHUD::FixedUpdate(float dt)
{
	GameObject::FixedUpdate(dt);
}

void UiHUD::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);
	textScore.Draw(window);
	textHighScore.Draw(window);
	textAmmo.Draw(window);
	textWave.Draw(window);
	textZombieCount.Draw(window);
	ammoIcon.Draw(window);
	window.draw(gaugeHp);
}

void UiHUD::SetScore(int score)
{
	this->score = score;
	textScore.SetText(formatScore + std::to_string(score));
}

void UiHUD::SetHighScore(int score)
{
	this->highScore = score;
	textHighScore.SetText(formatHighScore + std::to_string(highScore));
}


void UiHUD::SetHp(int hp, int maxHp)
{
	float value = (float)hp / (float)maxHp;
	this->hp = hp;
	gaugeHp.setSize({ gaugeHpSize.x * value , gaugeHpSize.y });
}


void UiHUD::SetWave(int wave)
{
	this->wave = wave;
	textWave.SetText(formatWave + std::to_string(this->wave));
}


void UiHUD::SetZombieCount(int zombieCount)
{
	this->zombieCount = zombieCount;
	textZombieCount.SetText(formatZombieCount + std::to_string(this->zombieCount));
}

void UiHUD::SetResolution(const sf::Vector2f& resolution)
{
}

void UiHUD::SetAmmo(int current, int gunCap)
{
	this->currentAmmo = current;
	this->gunCapacity = gunCap;

	textAmmo.SetText(std::to_string(currentAmmo) + "/" + std::to_string(gunCapacity));
}
