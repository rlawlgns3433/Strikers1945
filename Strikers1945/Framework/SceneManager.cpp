#include "pch.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "SceneTitle.h"

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::Init()
{
	Release();

	LoadAllResources();

	scenes.push_back(new SceneTitle(SceneIDs::SceneTitle));
	scenes.push_back(new SceneGame(SceneIDs::SceneGame));

	for (auto scene : scenes)
	{
		scene->Init();
	}

	currentScene = startScene;
	scenes[(int)currentScene]->Enter();
}

void SceneManager::Release()
{
	for (auto scene : scenes)
	{
		scene->Release();
		delete scene;
	}
	scenes.clear();
}

void SceneManager::LoadAllResources()
{

	for (const auto& name : TextureResourceNames)
	{
		TEXTURE_MANAGER.Load(name);
	}

	for (const auto& name : FontResourceNames)
	{
		FONT_MANAGER.Load(name);
	}

	for (const auto& name : SoundResourceNames)
	{
		SOUND_BUFFER_MANAGER.Load(name);
	}
}

void SceneManager::ChangeScene(SceneIDs id)
{
	scenes[(int)currentScene]->Exit();
	currentScene = id;
	scenes[(int)currentScene]->Enter();
}

void SceneManager::Update(float dt)
{
	scenes[(int)currentScene]->Update(dt);
}

void SceneManager::LateUpdate(float dt)
{
	scenes[(int)currentScene]->LateUpdate(dt);
}

void SceneManager::FixeUpdate(float dt)
{
	scenes[(int)currentScene]->FixedUpdate(dt);
}

void SceneManager::Draw(sf::RenderWindow& window)
{
	scenes[(int)currentScene]->Draw(window);
}