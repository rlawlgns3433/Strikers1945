#pragma once
#include "SpriteGo.h"

class SceneGame;

class Background : public SpriteGo
{

protected :
	Background(const Background&)					= delete;
	Background(Background&&)						= delete;
	Background& operator=(const Background&)		= delete;
	Background& operator=(Background&&)				= delete;

	sf::Clock clock;
	SceneGame* sceneGame = nullptr;

public :
	Background(const std::string& name = "background");
	~Background() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

};

