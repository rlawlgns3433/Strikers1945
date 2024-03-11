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

SceneGame::SceneGame(SceneIDs id) 
    : Scene(id)
{
    windowX = FRAMEWORK.GetWindowSize().x;
    windowY = FRAMEWORK.GetWindowSize().y;
}

void SceneGame::Init()
{
    worldView.setSize(windowX, windowY);
    worldView.setCenter(0, 0);
    worldView.setViewport(sf::FloatRect(0.f, 0.1f, 1.f, 0.8f));
    uiView.setSize(windowX, windowY);
    uiView.setCenter(windowX * 0.5f, windowY * 0.5f);

    player = new AnimPlayer();
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

    Scene::Init(); // 모든 게임 오브젝트 Init()
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
}

void SceneGame::Enter()
{
	Scene::Enter();
    status = GameStatus::Game;
    player->SetActive(true);
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
            RemoveGameObject(projectile);
    }
    usingProjectileList.clear();

    for (auto& item : ItemList)
    {
        if (item != nullptr)
            RemoveGameObject(item);
    }
    ItemList.clear();

    Scene::Exit();

}

void SceneGame::Update(float dt)
{
    std::cout << unusingProjectileList.size() << " : " << unusingProjectileList.size() << " : ";
    std::cout << enemyList.size() << std::endl;

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

    if (InputManager::GetKeyDown(sf::Keyboard::Space))
    {
        textCountDown->SetText(std::to_string(--countDown));
        std::cout << clock.getElapsedTime().asSeconds() << std::endl;
        std::cout << countDown << std::endl;
        clock.restart();
    }

    textCountDown->SetActive(true);
    if (clock.getElapsedTime().asSeconds() > 1.f)
    {
        textCountDown->SetText(std::to_string(--countDown));
        std::cout << clock.getElapsedTime().asSeconds() << std::endl;
        std::cout << countDown << std::endl;
        clock.restart();
    }

    if (countDown <= 0)
    {
        SCENE_MANAGER.ChangeScene(SceneIDs::SceneTitle);
        Reset();
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