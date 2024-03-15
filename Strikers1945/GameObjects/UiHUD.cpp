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

	for (int i = 0; i < 10; ++i)
	{
		spriteLife = new SpriteGo("life");
		spriteLife->Init();
		spriteLife->SetTexture("graphics/Strikers1945/assets/Life.png");
		spriteLife->SetPosition({ i * 50.f, windowSize.y * 0.9f });

		spriteLifes.push_back(spriteLife);

		spriteBomb = new SpriteGo("bomb");
		spriteBomb->Init();
		spriteBomb->SetTexture("graphics/Strikers1945/assets/bombUI.png");
		spriteBomb->SetPosition({ i * 50.f, windowSize.y * 0.1f });
		spriteBomb->SetOrigin(Origins::TL);
		spriteBombs.push_back(spriteBomb);
	}

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));

	sf::Font& font = *FONT_MANAGER.GetResource("fonts/ttf/strikers1945.ttf");

	textScore.Set(font, "", 30, sf::Color::White);
	textScore.SetPosition({ windowSize.x * 0.5f, windowSize.y * 0.05f});
	textScore.SetOrigin(Origins::TC);
}

void UiHUD::Reset()
{
	GameObject::Reset();

	textScore.SetText(scoreFormat + std::to_string(player->GetScore()));

	for (int i = player->GetLife(); i < 10; ++i)
	{
		spriteLifes[i]->SetActive(false);
	}

	for (int i = player->GetBombItem(); i < 10; ++i)
	{
		spriteBombs[i]->SetActive(false);
	}
}

void UiHUD::Update(float dt)
{
	GameObject::Update(dt);

	textScore.Update(dt);
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

	for (auto life : spriteLifes)
	{
		if(life->GetActive())
			life->Draw(window);
	}

	for (auto bomb : spriteBombs)
	{
		if (bomb->GetActive())
			bomb->Draw(window);
	}
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
	lifes += add;

	if (lifes < 0) return;

	for (int i = 0; i < lifes; ++i)
	{
		spriteLifes[i]->SetActive(true);
	}

	for (int i = lifes; i < 10; ++i)
	{
		spriteLifes[i]->SetActive(false);
	}
}

void UiHUD::SetLifes(int lifes)
{
	if (lifes > 10)
	{
		this->lifes = 10;
	}
	else
	{
		this->lifes = lifes;
	}

	for (int i = 0; i < this->lifes; ++i)
	{
		spriteLifes[i]->SetActive(true);
	}

	for (int i = this->lifes; i < 10; ++i)
	{
		spriteLifes[i]->SetActive(false);
	}
}

void UiHUD::AddBombCount(int add)
{
	bombCount += add;

	for (int i = 0; i < bombCount; ++i)
	{
		spriteBombs[i]->SetActive(true);
	}

	for (int i = bombCount; i < 10; ++i)
	{
		spriteBombs[i]->SetActive(false);
	}
}

void UiHUD::SetBombCount(int bombCount)
{
	if (bombCount > 10)
	{
		this->bombCount = 10;
	}
	else
	{
		this->bombCount = bombCount;
	}

	for (int i = 0; i < this->bombCount; ++i)
	{
		spriteBombs[i]->SetActive(true);
	}

	for (int i = this->bombCount; i < 10; ++i)
	{
		spriteBombs[i]->SetActive(false);
	}
}
