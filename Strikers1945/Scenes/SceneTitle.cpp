#include "pch.h"
#include "SceneTitle.h"
#include "TextGo.h"
#include "SpriteGo.h"


SceneTitle::SceneTitle(SceneIDs id)
	: Scene(id)
{
	windowX = FRAMEWORK.GetWindowSize().x;
	windowY = FRAMEWORK.GetWindowSize().y;
}

void SceneTitle::Init()
{
	worldView.setSize(windowX, windowY);
	worldView.setCenter(0, 0);
	uiView.setSize(windowX, windowY);
	uiView.setCenter(windowX * 0.5f, windowY * 0.5f);
	

	Scene::Init();
}

void SceneTitle::Release()
{
	Scene::Release();
}

void SceneTitle::Enter()
{
	Scene::Enter();

	opening = new SpriteGo("opening");
	opening->SetTexture("graphics/Strikers1945/assets/opening.bmp");
	opening->SetPosition({ 0,0 });
	opening->SetOrigin(Origins::MC);
	AddGameObject(opening);

	textIntro = new TextGo("intro");
	textIntro->Set(*FONT_MANAGER.GetResource("fonts/ttf/strikers1945.ttf"), "Press Enter To Start", 60, sf::Color::Red);
	textIntro->SetPosition({ windowX / 2, windowY * 0.65f });
	textIntro->SetOrigin(Origins::MC);
	AddGameObject(textIntro, Layers::Ui);
}

void SceneTitle::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);

	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	time += dt;
	BlinkIntro(0.7f);

	if (InputManager::GetKeyDown(sf::Keyboard::Enter))
	{
		SCENE_MANAGER.ChangeScene(SceneIDs::SceneUpgrade);
		std::cout << "SCENETITLE" << std::endl;
	}
}

void SceneTitle::BlinkIntro(float blinkTime)
{
	this->blinkTime = blinkTime;

	if (time >= 0 && time < blinkTime)
	{
		textIntro->SetActive(true);
	}
	else if (time >= blinkTime && time < blinkTime * 2)
	{
		textIntro->SetActive(false);
	}
	else if (time >= blinkTime * 2)
	{
		time = 0.f;
	}
}
