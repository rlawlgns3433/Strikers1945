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

	if (sceneGame->GetStatus() == GameStatus::Game)
	{
		if (clock.getElapsedTime().asSeconds() > 0.01) {
			position.y += 1.0f; // 속도 조절을 위해 이 값을 변경할 수 있음
			sprite.setPosition(0, position.y);
			clock.restart();
		}
	}
}
