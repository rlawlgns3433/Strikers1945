#include "pch.h"
#include "UiHUD.h"

UiHUD::UiHUD(const std::string& name)
	: GameObject(name)
{
}

void UiHUD::Init()
{
	GameObject::Init();
}

void UiHUD::Reset()
{
	GameObject::Reset();
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
}

void UiHUD::SetScore(int score)
{
}

void UiHUD::AddLifes(int add)
{
}

void UiHUD::SetLifes(int lifes)
{
}

void UiHUD::AddBombCount(int add)
{
}

void UiHUD::SetBombCount(int bombCount)
{
}
