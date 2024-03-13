#include "pch.h"
#include "UiHUD.h"
#include "SceneGame.h"
#include "AnimPlayer.h"

UiHUD::UiHUD(const std::string& name)
	: GameObject(name)
{
	windowSize = FRAMEWORK.GetWindowSize();
}

void UiHUD::Init()
{
	//점수, 라이프 수, 폭탄 수, 일시 정지
	GameObject::Init();
	textScore.Init();
	textBombCount.Init();
	textLifes.Init();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));

	sf::Font& font = *FONT_MANAGER.GetResource("fonts/ttf/strikers1945.ttf");

	textScore.Set(font, "", 30, sf::Color::White);
	textBombCount.Set(font, "", 30, sf::Color::White);
	textLifes.Set(font, "", 30, sf::Color::White);

	textScore.SetPosition({ windowSize.x * 0.5f, windowSize.y * 0.05f});
	textBombCount.SetPosition({ 0,  windowSize.y * 0.1f });
	textLifes.SetPosition({ 0,  windowSize.y * 0.9f });

	textScore.SetOrigin(Origins::TC);
	textBombCount.SetOrigin(Origins::ML);
	textLifes.SetOrigin(Origins::ML);
}

void UiHUD::Reset()
{
	GameObject::Reset();

	textScore.SetText(scoreFormat + std::to_string(player->GetScore()));
	textBombCount.SetText(bombCountFormat + std::to_string(player->GetBombItem()));
	textLifes.SetText(lifesFormat + std::to_string(player->GetLife()));
}

void UiHUD::Update(float dt)
{
	GameObject::Update(dt);

	textScore.Update(dt);
	textBombCount.Update(dt);
	textLifes.Update(dt);
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
	textBombCount.Draw(window);
	textLifes.Draw(window);
}

void UiHUD::SetScore(int score)
{
	this->score = score;
	textScore.SetText(scoreFormat + std::to_string(this->score));
}

void UiHUD::AddScore(int add)
{
	this->score += add;
	textScore.SetText(scoreFormat + std::to_string(this->score));
}

void UiHUD::AddLifes(int add)
{
	this->lifes += add;
	textLifes.SetText(lifesFormat + std::to_string(this->lifes));
}

void UiHUD::SetLifes(int lifes)
{
	this->lifes = lifes;
	textLifes.SetText(lifesFormat + std::to_string(this->lifes));
}

void UiHUD::AddBombCount(int add)
{
	this->bombCount += add;
	textBombCount.SetText(bombCountFormat + std::to_string(bombCount));
}

void UiHUD::SetBombCount(int bombCount)
{
	this->bombCount = bombCount;
	textBombCount.SetText(bombCountFormat + std::to_string(bombCount));
}
