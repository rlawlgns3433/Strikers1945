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

	goldDisplay.SetFont(font);
	goldDisplay.SetTextSize(35); 
	goldDisplay.SetFillColor(sf::Color::Yellow);
	goldDisplay.SetPosition(10, 10);

	AddGameObject(&goldDisplay, Layers::Ui);

	powerUpButton = new Button();
	powerUpButton->SetButton({ 150, 80 }, { 100, windowSize.y * 0.6f }, buttonColor, Origins::MC);
	powerUpButton->SetButtonText(font, "Power Up", 35, sf::Color::White, { 100, windowSize.y * 0.6f }, Origins::MC);

	extraLifeButton = new Button();
	extraLifeButton->SetButton({ 150, 80 }, { 270, windowSize.y * 0.6f }, buttonColor, Origins::MC);
	extraLifeButton->SetButtonText(font, "Extra Life", 35, sf::Color::White, { 270, windowSize.y * 0.6f }, Origins::MC);

	bombButton = new Button();
	bombButton->SetButton({ 150, 80 }, { 440, windowSize.y * 0.6f }, buttonColor, Origins::MC);
	bombButton->SetButtonText(font, "Bomb", 35, sf::Color::White, { 440, windowSize.y * 0.6f }, Origins::MC);

	startGameButton = new Button();
	 
	startGameButton->SetButton({ 450, 80 }, { windowSize.x * 0.5f, windowSize.y * 0.8f }, buttonColor, Origins::MC);
	startGameButton->SetButtonText(font, "Start Game", 35, sf::Color::White, { windowSize.x * 0.5f, windowSize.y * 0.8f }, Origins::MC);

	AddGameObject(powerUpButton, Layers::Ui);
	AddGameObject(extraLifeButton, Layers::Ui);
	AddGameObject(bombButton, Layers::Ui); 
	AddGameObject(startGameButton, Layers::Ui);

	myFlight = new SpriteGo();
	myFlight->SetTexture("graphics/Strikers1945/MyFighter.png"); 
	myFlight->SetPosition({ windowSize.x * 0.5f, windowSize.y * 0.2f });
	myFlight->SetOrigin(Origins::MC);
	AddGameObject(myFlight, Layers::Ui);

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
	goldDisplay.SetText("Gold: " + std::to_string(currentGold)); 
}

void SceneUpgrade::Exit()
{
	Scene::Exit();

}

void SceneUpgrade::Update(float dt)
{
	Scene::Update(dt);

	startGameButton->SetButtonColor(buttonColor);
	powerUpButton->SetButtonColor(buttonColor);
	extraLifeButton->SetButtonColor(buttonColor); 
	bombButton->SetButtonColor(buttonColor);

	if (InputManager::GetMouseButtonDown(sf::Mouse::Left))
	{
		isButtonPressed = true;
		if (startGameButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			startGameButton->SetButtonColor(buttonColorPressed);
		}

		if (powerUpButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			powerUpButton->SetButtonColor(buttonColorPressed);
		}

		if (extraLifeButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			extraLifeButton->SetButtonColor(buttonColorPressed);
		}

		if (bombButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			bombButton->SetButtonColor(buttonColorPressed);
		}
	}
	else if (InputManager::GetMouseButtonUp(sf::Mouse::Left))
	{
		if (startGameButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			SaveGold();

			startGameButton->SetButtonColor(buttonColor);
			SCENE_MANAGER.ChangeScene(SceneIDs::SceneGame);
		}

		if (powerUpButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			UpgradePowerLevel();
			powerUpButton->SetButtonColor(buttonColor);
		}

		if (extraLifeButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			UpgradeExtraLifes();
			extraLifeButton->SetButtonColor(buttonColor);
		} 

		if (bombButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			UpgradeExtraBombs();
			bombButton->SetButtonColor(buttonColor);
		}

		goldDisplay.SetText("Gold: " + std::to_string(currentGold));
	}
	else
	{
		if (startGameButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			startGameButton->SetButtonColor(buttonColorFocused);


		}

		if (powerUpButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			powerUpButton->SetButtonColor(buttonColorFocused);
		}

		if (extraLifeButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			extraLifeButton->SetButtonColor(buttonColorFocused);
		}

		if (bombButton->GetGlobalBounds().contains(InputManager::GetMousePos()))
		{
			bombButton->SetButtonColor(buttonColorFocused);
		}
	}
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

void SceneUpgrade::UpgradePowerLevel()
{
	if (currentGold < 2000) return;
	if (extraPowerLevel >= 3) return;

	++extraPowerLevel;
	currentGold -= 2000;
}

void SceneUpgrade::UpgradeExtraLifes()
{
	if (currentGold < 4000) return;
	if (extraLifes >= 7) return;

	++extraLifes;
	currentGold -= 4000;
}

void SceneUpgrade::UpgradeExtraBombs()
{
	if (currentGold < 2000) return;
	if (extraBombs >= 7) return;

	++extraBombs;
	currentGold -= 2000;
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
