#include "pch.h"
#include "SceneDev1.h"
#include "AniTest.h"
//#include "AniTest2.h"

SceneDev1::SceneDev1(SceneIDs id)
	: Scene(id)
{
	windowX = FRAMEWORK.GetWindowSize().x;
	windowY = FRAMEWORK.GetWindowSize().y;
}

SceneDev1::~SceneDev1()
{
}

void SceneDev1::Init()
{
	worldView.setSize(windowX, windowY);
	worldView.setCenter(0, 0);
	uiView.setSize(windowX, windowY);
	uiView.setCenter(windowX * 0.5f, windowY * 0.5f);

	AddGameObject(new AniTest());
	//AddGameObject(new AniTest2());
	Scene::Init();
}

void SceneDev1::Release()
{
	Scene::Release();
}

void SceneDev1::Enter()
{
	Scene::Enter();
}

void SceneDev1::Exit()
{
	FRAMEWORK.SetTimeScale(1.f);
}

void SceneDev1::Update(float dt)
{
	Scene::Update(dt);
}