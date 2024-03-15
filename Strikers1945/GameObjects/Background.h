#pragma once
#include "SpriteGo.h"

class SceneGame;

class Background : public SpriteGo
{
public :
	enum GamePhase
	{
		None,
		CommonEnemyPhase, // 잡몹 나오는 단계
		MidBossPhase,     // 세미 보스 나오는 단계
		BossPhase          // 보스 나오는 단계
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
	void SetPhase(GamePhase phase);
};

