#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneGame;

class AnimBomb : public SpriteGo
{
protected :
	AnimBomb(const AnimBomb&)				= delete;
	AnimBomb(AnimBomb&&)					= delete;
	AnimBomb& operator=(const AnimBomb&)	= delete;
	AnimBomb& operator=(AnimBomb&&)			= delete;

	SceneGame* sceneGame = nullptr;;
	Animator animator;

	std::string animationClipId = "animation/BombExplosion.csv";

public:
	AnimBomb(const std::string& name = "bomb");
	~AnimBomb() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void BombTerminateEvent();
	void BombKillEnemyEvent();
};