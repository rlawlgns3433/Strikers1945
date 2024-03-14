#pragma once

class SpriteGo;
class TextGo;

class SceneDev1 : public Scene
{

	SpriteGo* spriteGoBackground;
	TextGo* textIntro;
	TextGo* textTitle;

	float windowX = 0.f;
	float windowY = 0.f;

public:
	SceneDev1(SceneIDs id);
	virtual ~SceneDev1();

	SceneDev1(const SceneDev1&) = delete;
	SceneDev1(SceneDev1&&) = delete;
	SceneDev1& operator=(const SceneDev1&) = delete;
	SceneDev1& operator=(SceneDev1&&) = delete;

	// SceneTitle을(를) 통해 상속됨
	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;
};