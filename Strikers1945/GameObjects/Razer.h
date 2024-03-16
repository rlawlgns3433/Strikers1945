#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneGame;

class Razer : public SpriteGo
{
protected :
	Razer(const Razer&) = delete;
	Razer(Razer&&) = delete;
	Razer& operator=(const Razer&) = delete;
	Razer& operator=(Razer&&) = delete;

	SceneGame* sceneGame = nullptr;
	Animator animator;

public :
	Razer(const std::string& name = "razer");
	~Razer() override = default;

	void Init() override;
	void Reset() override;;
	void Update(float dt) override;

	const Animator& GetAnimator() const { return animator; }
};