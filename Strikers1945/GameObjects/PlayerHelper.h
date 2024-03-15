#pragma once
#include "SpriteGo.h"
#include "HelperBullet.h"
#include "SceneGame.h"

class AnimPlayer;


class PlayerHelper : public SpriteGo
{
protected :
	PlayerHelper(const PlayerHelper&) = delete;
	PlayerHelper(PlayerHelper&&) = delete;
	PlayerHelper& operator=(const PlayerHelper&) = delete;
	PlayerHelper& operator=(PlayerHelper&&) = delete;

	SceneGame* sceneGame = nullptr;
	AnimPlayer* player = nullptr;

	float attackTimer = 0.f;
	float attackInterval = 1.f;
	
	std::list<HelperBullet*> usingHelperBulletlist;
	std::list<HelperBullet*> unusingHelperBulletlist;

	sf::Vector2f offset;

public :
	PlayerHelper(const std::string& name = "playerhelper");
	~PlayerHelper() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Shoot();
	void SetOffset(const sf::Vector2f& offset);
};