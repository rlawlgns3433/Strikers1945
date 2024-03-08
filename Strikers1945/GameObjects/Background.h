#pragma once
#include "SpriteGo.h"

class SceneGame;

class Background : public SpriteGo
{
public :
	enum GamePhase
	{
		COMMON_ENEMY_PHASE, // ��� ������ �ܰ�
		MID_BOSS_PHASE,     // ���� ���� ������ �ܰ�
		BOSS_PHASE          // ���� ������ �ܰ�
	};


protected :
	Background(const Background&)					= delete;
	Background(Background&&)						= delete;
	Background& operator=(const Background&)		= delete;
	Background& operator=(Background&&)				= delete;

	GamePhase phase;
	SceneGame* sceneGame = nullptr;

	sf::Clock clock;

public :
	Background(const std::string& name = "background");
	~Background() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	GamePhase GetPhase() const { return phase; }
};

