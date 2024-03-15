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

	GetGold();

	goldDisplay.SetFont(font);
	goldDisplay.SetTextSize(35);
	goldDisplay.SetFillColor(sf::Color::Yellow);
	goldDisplay.SetPosition(10, 10);
	goldDisplay.SetText("Gold: " + std::to_string(currentGold));

	powerUpButton = new Button();
	powerUpButton->SetButton({ 150, 80 }, { 100, 400 }, sf::Color(127,127,127), Origins::MC);
	powerUpButton->SetButtonText(font, "Power Up", 35, sf::Color::White, { 100, 400 }, Origins::MC);

	extraLifeButton = new Button();
	extraLifeButton->SetButton({ 150, 80 }, { 270, 400 }, sf::Color(127, 127, 127), Origins::MC);
	extraLifeButton->SetButtonText(font, "Extra Life", 35, sf::Color::White, { 270, 400 }, Origins::MC);

	bombButton = new Button();
	bombButton->SetButton({ 150, 80 }, { 440, 400 }, sf::Color(127, 127, 127), Origins::MC);
	bombButton->SetButtonText(font, "Bomb", 35, sf::Color::White, { 440, 400 }, Origins::MC);

	startGameButton = new Button();
	 
	startGameButton->SetButton({ 450, 80 }, { windowSize.x * 0.5f, windowSize.y * 0.7f }, sf::Color(127, 127, 127), Origins::MC);
	startGameButton->SetButtonText(font, "Start Game", 35, sf::Color::White, { windowSize.x * 0.5f, windowSize.y * 0.7f }, Origins::MC);


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
}

void SceneUpgrade::Exit()
{
	Scene::Exit();
}

void SceneUpgrade::Update(float dt)
{
	Scene::Update(dt);

	if (InputManager::GetMouseButtonDown(sf::Mouse::Left))
	{
		if (startGameButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			SaveGold();
			SCENE_MANAGER.ChangeScene(SceneIDs::SceneGame);
		}
		
		if (powerUpButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			UpgradePowerLevel();
		}

		if (extraLifeButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			UpgradeExtraLifes();
		}

		if (bombButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			UpgradeExtraBombs();
		}
	}
}

void SceneUpgrade::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

int SceneUpgrade::GetGold()
{
	std::ifstream input("gold.txt"); // Open the file for reading

	if (input.is_open())
	{
		input >> currentGold; // Attempt to read the gold amount
	}
	input.close(); // Close the file

	return currentGold; // Return the read (or default) value
}

void SceneUpgrade::UpgradePowerLevel()
{
	if (extraPowerLevel >= 3) return;

	++extraPowerLevel;
	// 재화 감소
	// 재화 저장
}

void SceneUpgrade::UpgradeExtraLifes()
{
	if (extraLifes >= 7) return;
	++extraLifes;
	// 재화 감소
	// 재화 저장
}

void SceneUpgrade::UpgradeExtraBombs()
{
	if (extraBombs >= 7) return;
	++extraBombs;
	// 재화 감소
	// 재화 저장
}

std::vector<int> SceneUpgrade::GetExtraStat()
{
	std::vector<int> extraStats;
	extraStats.push_back(extraPowerLevel);
	extraStats.push_back(extraLifes);
	extraStats.push_back(extraBombs);

	return extraStats;
}

void SceneUpgrade::SaveGold()
{
	std::ofstream input;
	input.open("gold.txt");
	if (input.is_open())
	{
		input << currentGold;
	}
}
