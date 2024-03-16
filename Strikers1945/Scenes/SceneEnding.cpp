#include "pch.h"
#include "SceneEnding.h"
#include "SpriteGo.h"
#include "SceneGame.h"
#include "AnimPlayer.h"
#include "TextGo.h"

SceneEnding::SceneEnding(SceneIDs id)
	: Scene(id)
{
    windowSize = (sf::Vector2f)(FRAMEWORK.GetWindowSize());
}

void SceneEnding::Init()
{
    font = *FONT_MANAGER.GetResource("fonts/ttf/strikers1945.ttf");

    worldView.setSize(windowSize);
    worldView.setCenter(0, 0);
    worldView.setViewport(sf::FloatRect(0.f, 0.1f, 1.f, 0.8f));
    uiView.setSize(windowSize.x, windowSize.y);
    uiView.setCenter(windowSize.x * 0.5f, windowSize.y * 0.5f);

    SpriteGo* spriteGameOver = new SpriteGo("spritegameover");
    spriteGameOver->SetTexture("graphics/Strikers1945/assets/GameOver.png");
    spriteGameOver->SetOrigin(Origins::MC);
    spriteGameOver->SetPosition({ 0, -300 });

    AddGameObject(spriteGameOver);

	Scene::Init();
}

void SceneEnding::Release()
{
	Scene::Release();
}

void SceneEnding::Enter()
{
	Scene::Enter();
    ranking = GetScores();
    for (int i = 0; i < ranking.size(); ++i)
    {
        std::cout << (*(ranking.begin() + i)).first << " : " << (*(ranking.begin() + i)).second << std::endl;
    }

    for (int i = 0; i < ranking.size(); ++i)
    {
        textRanking.push_back(new TextGo("textranking"));
        textRanking[i]->Init();
        textRanking[i]->Reset();
        textRanking[i]->Set(font, (*(ranking.begin()+i)).first + "\t" + std::to_string((*(ranking.begin() + i)).second), 40, sf::Color::White);
        textRanking[i]->SetOrigin(Origins::MC);
        textRanking[i]->SetPosition({ 0, i * 100.f });

        AddGameObject(textRanking[i]);
    }
}

void SceneEnding::Exit()
{
    for (auto text : textRanking)
    {
        RemoveGameObject(text);
    }

    textRanking.clear();
    ranking.clear();
	Scene::Exit();
}

void SceneEnding::Update(float dt)
{
    Scene::Update(dt);
    SetStatus(status);

    if ((*(ranking.end() - 1)).second < playerScore)
    {
        if (clock.getElapsedTime().asSeconds() > nameInputInterval)
        {
            (*(ranking.end() - 1)).first = (*(textRanking.end() - 1))->GetText();
            if ((*(ranking.end() - 1)).first.size() >= 3)
            {
                nameInputInterval = 0.f;

                (*(textRanking.end() - 1))->SetFocused(false);
                (*(ranking.end() - 1)).second = playerScore;
                SortRanking();
            }
        }
    }

    if (InputManager::GetKeyDown(sf::Keyboard::Enter))
    {
        SCENE_MANAGER.ChangeScene(SceneIDs::SceneTitle);
    }
}

void SceneEnding::SetStatus(GameStatus newStatus)
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

std::vector<std::pair<std::string, int>>& SceneEnding::GetScores()
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

void SceneEnding::SortRanking()
{
    std::sort(ranking.begin(), ranking.end(),
        [](std::pair<std::string, int> lhs, std::pair<std::string, int> rhs)
        {
            return lhs.second > rhs.second;
        });
}