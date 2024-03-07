#pragma once
#include "SpriteGo.h"
#include "SceneGame.h"
#include "AnimPlayer.h"

class SceneGame;

class Bullet : public SpriteGo
{
protected :
	Bullet(const Bullet&)				= delete;
	Bullet(Bullet&&)					= delete;
	Bullet& operator=(const Bullet&)	= delete;
	Bullet& operator=(Bullet&&)			= delete;
	
	SceneGame* sceneGame = nullptr;
	sf::Vector2f direction = { 0, -1 };
	AnimPlayer* player = nullptr;
	
	float time = 0.f;
	float speed = 800.f;
	int damage = 20;

public :
	Bullet(const std::string& name = "bullet");
	~Bullet() override					= default;

	void Init() override;
	void Reset() override;;
	void Update(float dt) override;
	void FixedUpdate(float dt);
};