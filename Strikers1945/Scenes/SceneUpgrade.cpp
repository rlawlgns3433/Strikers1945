#include "pch.h"
#include "SceneUpgrade.h"
#include "Button.h"

SceneUpgrade::SceneUpgrade(SceneIDs id)
	: Scene(id)
{
	windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
}

void SceneUpgrade::Init()
{
	font = *FONT_MANAGER.GetResource("fonts/ttf/strikers1945.ttf");

	worldView.setSize(windowSize);
	worldView.setCenter(0, 0);
	uiView.setSize(windowSize);
	uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

	goldDisplay = new TextGo();
	goldDisplay->Set(font, std::to_string(currentGold), 35, sf::Color::Yellow);
	goldDisplay->SetPosition(10, 10);
	AddGameObject(goldDisplay, Layers::Ui);

	powerUpButton = new Button(Button::ButtonIdentifier::PowerUp);
	powerUpButton->SetButton({ 150, 80 }, { 100, windowSize.y * 0.6f }, buttonColor, Origins::MC);
	powerUpButton->SetButtonText(font, "Power Up", 35, sf::Color::White, { 100, windowSize.y * 0.6f }, Origins::MC);

	extraPowerCost = new TextGo();
	extraPowerCost->Set(font, "2000G", 25, sf::Color::Yellow);
	extraPowerCost->SetPosition({ 100, windowSize.y * 0.65f });
	extraPowerCost->SetOrigin(Origins::MC);

	extraLifeButton = new Button(Button::ButtonIdentifier::ExtraLife);
	extraLifeButton->SetButton({ 150, 80 }, { 270, windowSize.y * 0.6f }, buttonColor, Origins::MC);
	extraLifeButton->SetButtonText(font, "Extra Life", 35, sf::Color::White, { 270, windowSize.y * 0.6f }, Origins::MC);

	extraLifesCost = new TextGo();
	extraLifesCost->Set(font, "4000G", 25, sf::Color::Yellow);
	extraLifesCost->SetPosition({ 270, windowSize.y * 0.65f });
	extraLifesCost->SetOrigin(Origins::MC);

	bombButton = new Button(Button::ButtonIdentifier::Bomb);
	bombButton->SetButton({ 150, 80 }, { 440, windowSize.y * 0.6f }, buttonColor, Origins::MC);
	bombButton->SetButtonText(font, "Bomb", 35, sf::Color::White, { 440, windowSize.y * 0.6f }, Origins::MC);

	extraBombsCost = new TextGo();
	extraBombsCost->Set(font, "2000G", 25, sf::Color::Yellow);
	extraBombsCost->SetPosition({ 440, windowSize.y * 0.65f });
	extraBombsCost->SetOrigin(Origins::MC);

	startGameButton = new Button(Button::ButtonIdentifier::StartGame);
	startGameButton->SetButton({ 450, 80 }, { windowSize.x * 0.5f, windowSize.y * 0.8f }, buttonColor, Origins::MC);
	startGameButton->SetButtonText(font, "Start Game", 35, sf::Color::White, { windowSize.x * 0.5f, windowSize.y * 0.8f }, Origins::MC);

	AddGameObject(extraPowerCost, Layers::Ui);
	AddGameObject(extraLifesCost, Layers::Ui);
	AddGameObject(extraBombsCost, Layers::Ui);

	AddGameObject(powerUpButton, Layers::Ui);
	AddGameObject(extraLifeButton, Layers::Ui);
	AddGameObject(bombButton, Layers::Ui); 
	AddGameObject(startGameButton, Layers::Ui);

	Scene::Init();
}
 
void SceneUpgrade::Release()
{
}

void SceneUpgrade::Enter()
{
	Scene::Enter();
	extraPowerLevel = 0;
	extraLifes = 0;
	extraBombs = 0;

	GetGold();
	goldDisplay->SetText("Gold: " + std::to_string(currentGold)); 
}

void SceneUpgrade::Exit()
{
	Scene::Exit();
}

void SceneUpgrade::Update(float dt)
{
	Scene::Update(dt);
	goldDisplay->SetText("Gold: " + std::to_string(currentGold));
}

void SceneUpgrade::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

int SceneUpgrade::GetGold()
{
	std::ifstream input("gold.txt");

	if (input.is_open())
	{
		input >> currentGold; 
	}
	input.close(); 

	return currentGold;
}
std::vector<int> SceneUpgrade::GetExtraStat()
{
	std::vector<int> extraStats;
	extraStats.push_back(extraPowerLevel);
	extraStats.push_back(extraLifes);
	extraStats.push_back(extraBombs);

	return extraStats;
}