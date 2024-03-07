#pragma once

class SpriteGo;
class TextGo;

class SceneTitle : public Scene
{
protected:
	SpriteGo* opening = nullptr;
	TextGo* textIntro = nullptr;

	float windowX;
	float windowY;

	float blinkTime = 0.7f;
	float time = 0.f;

public:
	SceneTitle(SceneIDs id);
	virtual ~SceneTitle();

	SceneTitle(const SceneTitle&) = delete;
	SceneTitle(SceneTitle&&) = delete;
	SceneTitle& operator=(const SceneTitle&) = delete;
	SceneTitle& operator=(SceneTitle&&) = delete;

	// SceneTitle을(를) 통해 상속됨
	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;
	void Update(float dt) override;

	void BlinkIntro(float blinkTime);
};