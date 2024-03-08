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
}

void Background::Update(float dt)
{
	SpriteGo::Update(dt);

	std::cout << position.y << std::endl;
	if (sceneGame->GetStatus() == GameStatus::Game)
	{
		if (clock.getElapsedTime().asSeconds() > 0.01) {
			position.y += 200.0f; // 속도 조절을 위해 이 값을 변경할 수 있음
			sprite.setPosition(0, position.y);
			clock.restart();
		}
	}

	if (position.y > sprite.getTexture()->getSize().y - FRAMEWORK.GetWindowSize().y * 0.5f) SetPosition(sf::Vector2f{ position.x, sprite.getTexture()->getSize().y - FRAMEWORK.GetWindowSize().y * 0.5f });

	if (position.y < 4000.f) phase = GamePhase::COMMON_ENEMY_PHASE;
	else if (position.y < 9000.f) phase = GamePhase::MID_BOSS_PHASE;
	else phase = GamePhase::BOSS_PHASE;
}
