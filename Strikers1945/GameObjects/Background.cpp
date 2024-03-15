#include "pch.h"
#include "Background.h"
#include "SceneGame.h"

Background::Background(const std::string& name)
	:SpriteGo(name)
{
}

void Background::Init()
{
	SpriteGo::Init();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
}

void Background::Reset()
{
	SetTexture("graphics/Strikers1945/assets/Stage_1.png");
	SetPosition({ 0,540 });
	SetOrigin(Origins::BC);
	SetSortLayer(-1);
	sceneGame->ResortGameObject(this);
	sprite.setPosition(0, 0);
}

void Background::Update(float dt)
{
	SpriteGo::Update(dt);

	if (sceneGame->GetStatus() == GameStatus::Game)
	{
		if (clock.getElapsedTime().asSeconds() > 0.01) {
			position.y += 1.f;
			sprite.setPosition(0, position.y);
			clock.restart();
		}
	}

	if (position.y > sprite.getTexture()->getSize().y - FRAMEWORK.GetWindowSize().y * 0.5f)
	{
		SetPosition(sf::Vector2f{ position.x, sprite.getTexture()->getSize().y - FRAMEWORK.GetWindowSize().y * 0.5f });
	}

	if (position.y < 4000.f) phase = GamePhase::CommonEnemyPhase;
	else if (position.y < 9000.f) phase = GamePhase::MidBossPhase;
	else phase = GamePhase::BossPhase;
}

void Background::SetPhase(GamePhase phase)
{
	switch (phase)
	{
	case Background::CommonEnemyPhase:
		position.y = 540.f;
		break;
	case Background::MidBossPhase:
		position.y = 4000.f;
		break;
	case Background::BossPhase:
		position.y = 9000.f;
		break;
	}
}
