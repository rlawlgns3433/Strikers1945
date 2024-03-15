#include "pch.h"
#include "SceneGame.h"
#include "UiHUD.h"
#include "Tilemap.h"
#include "AnimPlayer.h"
#include "AniTest.h"
#include "EnemySpawner.h"
#include "BackGround.h"
#include "Enemy.h"
#include "EnemyProjectile.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "Item.h"
#include "SceneUpgrade.h"

SceneGame::SceneGame(SceneIDs id) 
    : Scene(id)
{
    windowX = FRAMEWORK.GetWindowSize().x;
    windowY = FRAMEWORK.GetWindowSize().y;
}

void SceneGame::Init()
{
    font = *FONT_MANAGER.GetResource("fonts/ttf/strikers1945.ttf");

    worldView.setSize(windowX, windowY);
    worldView.setCenter(0, 0);
    worldView.setViewport(sf::FloatRect(0.f, 0.1f, 1.f, 0.8f));
    uiView.setSize(windowX, windowY);
    uiView.setCenter(windowX * 0.5f, windowY * 0.5f);

    ranking = GetScores();
    gold = GetGold();

    fadeWindow = sf::RectangleShape((sf::Vector2f)(FRAMEWORK.GetWindowSize()));
    fadeWindow.setFillColor(sf::Color(0, 0, 0, 0));

    player = new AnimPlayer(PlayerType::F_4);
    AddGameObject(player);

    enemySpawner = new EnemySpawner();
    AddGameObject(enemySpawner);
    
    background = new Background();
    AddGameObject(background);

    pauseWindow = new SpriteGo("pauseWindow");
    pauseWindow->SetTexture("graphics/Strikers1945/assets/pauseWindow.png");
    pauseWindow->SetPosition({ windowX * 0.5f, windowY * 0.5f });
    pauseWindow->SetOrigin(Origins::MC);
    pauseWindow->SetActive(false);
    AddGameObject(pauseWindow, Layers::Ui);

    textCountDown = new TextGo("countdown");
    textCountDown->Set(*FONT_MANAGER.GetResource("fonts/ttf/strikers1945.ttf"), std::to_string(countDown), 100, sf::Color::Red);
    textCountDown->SetPosition({ windowX * 0.5f, windowY * 0.5f });
    textCountDown->SetOrigin(Origins::MC);
    textCountDown->SetActive(false);
    AddGameObject(textCountDown, Layers::Ui);

    saveName = new TextGo();
    saveName->Init();
    saveName->Reset();
    saveName->Set(font, saveNameFormat, 50, sf::Color::White);
    saveName->SetOrigin(Origins::MC);
    saveName->SetPosition({ windowX * 0.5f, windowY * 0.5f });
    saveName->SetActive(false);
    AddGameObject(saveName, Layers::Ui);

    Scene::Init();
}

void SceneGame::Release()
{
    Scene::Release();
}

void SceneGame::Reset()
{
    player->Reset();
    background->Reset();
    textCountDown->Set(*FONT_MANAGER.GetResource("fonts/ttf/strikers1945.ttf"), std::to_string(countDown), 100, sf::Color::Red);
    countDown = 10;
    nameInputInterval = 10.f;
    alpha = 0.f;
    fadeWindow.setFillColor(sf::Color(0, 0, 0, alpha));
    saveName->SetText(saveNameFormat);
}

void SceneGame::Enter()
{
	Scene::Enter();
    status = GameStatus::Game;
    sceneUpgrade = dynamic_cast<SceneUpgrade*>(SCENE_MANAGER.GetScene(SceneIDs::SceneUpgrade));

    for (auto e : sceneUpgrade->GetExtraStat())
    {
        std::cout << e << std::endl;
    }

    player->SetActive(true);
    player->SetExtraStat(sceneUpgrade->GetExtraStat());
}

void SceneGame::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);

    for (auto& enemy : enemyList)
    {
        if (enemy != nullptr)
            RemoveGameObject(enemy);
    }
    enemyList.clear();

    for (auto& projectile : usingProjectileList)
    {
        if (projectile != nullptr)
            projectile->SetActive(false);
    }
    usingProjectileList.clear();

    for (auto& item : ItemList)
    {
        if (item != nullptr)
            item->SetActive(false);
    }
    ItemList.clear();


    Scene::Exit();
}

void SceneGame::Update(float dt)
{
    Scene::Update(dt);
    SetStatus(status);

    switch (status)
    {
    case GameStatus::Awake:
        UpdateAwake(dt);
        break;
    case GameStatus::Game:
        UpdateGame(dt);
        break;
    case GameStatus::GameOver:
        UpdateGameover(dt);
        break;
    case GameStatus::Pause:
        UpdatePause(dt);
        break;
    default:
        break;
    }
}

void SceneGame::UpdateAwake(float dt)
{
}

void SceneGame::UpdateGame(float dt)
{
    textCountDown->SetActive(false);

    if (InputManager::GetKeyDown(sf::Keyboard::Escape))
    {
        status = GameStatus::Pause;
        pauseWindow->SetActive(true);
    }

    if (InputManager::GetKeyDown(sf::Keyboard::F2))
    {
        player->SetCheatMode();
    }

    if (InputManager::GetKeyDown(sf::Keyboard::F5))
    {
        background->SetPhase(Background::CommonEnemyPhase);
    }
    if (InputManager::GetKeyDown(sf::Keyboard::F6))
    {
        background->SetPhase(Background::MidBossPhase);
    }
    if (InputManager::GetKeyDown(sf::Keyboard::F7))
    {
        background->SetPhase(Background::BossPhase);
    }

    auto it = usingProjectileList.begin();
    while (it != usingProjectileList.end())
    {
        auto projectile = *it;
        if (!projectile->GetActive())
        {
            it = usingProjectileList.erase(it);
            unusingProjectileList.push_back(projectile);
        }
        else
        {
            ++it;
        }
    }
}

void SceneGame::UpdateGameover(float dt)
{
    textCountDown->SetActive(true);

    if (InputManager::GetKeyDown(sf::Keyboard::Space) && countDown > 0)
    {
        textCountDown->SetText(std::to_string(--countDown));
        alpha = ((10 - countDown) / 10.f) * 255.f;
        fadeWindow.setFillColor(sf::Color(0, 0, 0, alpha));
        clock.restart();
    }

    if (clock.getElapsedTime().asSeconds() > 1.f && countDown > 0)
    {
        textCountDown->SetText(std::to_string(--countDown));
        alpha = ((10 - countDown) / 10.f) * 255.f;
        fadeWindow.setFillColor(sf::Color(0, 0, 0, alpha));
        clock.restart();
    }

    if (countDown <= 0)
    {
        if (ranking.size() < 2 || (*(ranking.begin() + 2)).second < player->GetScore())
        {
            fadeWindow.setFillColor(sf::Color(0, 0, 0, 0));
            saveName->SetSortLayer(1);
            ResortGameObject(saveName);
            saveName->SetActive(true);
            isNewRecord = true;
            if (clock.getElapsedTime().asSeconds() <= nameInputInterval)
            {
                saveName->SetFocused(true);

                if (saveName->GetText().size() >= 15)
                {
                    std::cout << saveName->GetText() << std::endl;
                    nameInputInterval = 0.f;
                }
            }
            else
            {
                SaveHighScore();
                saveName->SetFocused(false);
                saveName->SetActive(false);
                isNewRecord = false;
            }
        }

        if (!isNewRecord)
        {
            GetReward();
            SetStatus(GameStatus::GameOver);
            Reset();
            SCENE_MANAGER.ChangeScene(SceneIDs::SceneEnding);
        }
    }
    else
    {
        if (InputManager::GetKeyDown(sf::Keyboard::Num5) || InputManager::GetKeyDown(sf::Keyboard::Numpad5))
        {
            isCoinInserted = true;
        }
    }

    if (isCoinInserted)
    {
        player->Reset();
        countDown = 10;
        nameInputInterval = 10.f;
        alpha = 0.f;
        fadeWindow.setFillColor(sf::Color(0, 0, 0, alpha));
        saveName->SetText(saveNameFormat);
        clock.restart();
        isCoinInserted = false;
        SetStatus(GameStatus::Game);
    }
}

void SceneGame::UpdatePause(float dt)
{
    if (InputManager::GetKeyDown(sf::Keyboard::Escape))
    {
        status = GameStatus::Game;
        pauseWindow->SetActive(false);
    }
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
    window.draw(fadeWindow);
}

void SceneGame::SetStatus(GameStatus newStatus)
{
    GameStatus prevStatus = status;

    status = newStatus;

    switch (status)
    {
    case GameStatus::Awake:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    case GameStatus::Game:
        FRAMEWORK.SetTimeScale(1.f);
        break;
    case GameStatus::GameOver:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    case GameStatus::Pause:
        FRAMEWORK.SetTimeScale(0.f);
        break;
    }
}

void SceneGame::GetReward()
{
    gold += player->GetScore() / 100;

    std::ofstream input;
    input.open("gold.txt");
    if (input.is_open())
    {
        input << gold;
    }
}

void SceneGame::SaveHighScore()
{
    std::ifstream file("highScore.txt", std::ios_base::app);

    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::vector<std::string> lines;
    std::string line;

    std::ofstream input;
    input.open("highScore.txt", std::ios::app);
    if (input.is_open())
    {
        input << saveName->GetText().substr(12, 15) <<  player->GetScore() << '\n' /*<< playTimer*/;
    }

    input.close();
}

std::vector<std::pair<std::string, int>>& SceneGame::GetScores()
{
    std::ifstream file("highScore.txt");

    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
    }

    std::string rank;
    int i = 0;
    while (file >> rank)
    {
        std::string name = rank.substr(0, 3);
        int score = std::stoi(rank.substr(3));

        ranking.push_back(std::make_pair(name, score));
        ++i;
    }
    file.close();

    SortRanking();

    return ranking;
}

int SceneGame::GetGold()
{
    std::ifstream output;
    output.open("gold.txt", std::ios::app);

    if (output.is_open())
    {
        output >> gold;
    }
    output.close();

    return gold;
}

int SceneGame::GetHighScore()
{
    std::ifstream output;
    output.open("highScore.txt", std::ios::app);

    if (output.is_open())
    {
        output >> hiScore;
    }
    output.close();

    return hiScore;
}

void SceneGame::SortRanking()
{
    std::sort(ranking.begin(), ranking.end(),
        [](std::pair<std::string, int> lhs, std::pair<std::string, int> rhs)
        {
            return lhs.second > rhs.second;
        });
}