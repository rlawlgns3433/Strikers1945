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
    uiView.setSize(windowX, windowY);
    uiView.setCenter(windowX * 0.5f, windowY * 0.5f);

    player = new AnimPlayer();
    AddGameObject(player);

    enemySpawner = new EnemySpawner();
    AddGameObject(enemySpawner);
    
    background = new Background();
    AddGameObject(background);

    Scene::Init(); // 모든 게임 오브젝트 Init()
}

void SceneGame::Release()
{
    Scene::Release();
}

void SceneGame::Reset()
{
}

void SceneGame::Enter()
{
	Scene::Enter();
}

void SceneGame::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);
}

void SceneGame::Update(float dt)
{
    Scene::Update(dt);
    SetStatus(status);

    std::cout << Utils::Random::RandomValue() << std::endl;


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
    //sf::Vector2f worldViewCenter = worldView.getCenter();
    //worldViewCenter = Utils::Vector2::Lerp(worldViewCenter, player->GetPosition(), dt * 5);
    //worldView.setCenter(worldViewCenter);

    //worldView.setCenter(player->GetPosition());


}

void SceneGame::UpdateGameover(float dt)
{
}

void SceneGame::UpdatePause(float dt)
{
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

bool SceneGame::IsInTilemap(const sf::Vector2f& point)
{
    sf::FloatRect rect = tilemap->GetGlobalBounds();
    rect = Utils::MyMath::ResizeRect(rect, tilemap->GetCellSize() * -2.f);

    return rect.contains(point);
}

sf::Vector2f SceneGame::ClampByTilemap(const sf::Vector2f& point)
{
    sf::FloatRect rect = tilemap->GetGlobalBounds();
    rect = Utils::MyMath::ResizeRect(rect, tilemap->GetCellSize() * -2.f);
    return Utils::MyMath::Clamp(point, rect);
}