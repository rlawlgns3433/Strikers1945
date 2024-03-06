#include "pch.h"
#include "AnimPlayer.h"
#include "SceneGame.h"
#include "Bullet.h"

AnimPlayer::AnimPlayer(const std::string& name)
	: SpriteGo(name)
{
}

void AnimPlayer::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);

	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", false, });
	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", true });


	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
	SetPosition({ 0, 450.f });
}

void AnimPlayer::Reset()
{
	animator.Play("animation/Player/Idle.csv");
	SetOrigin(Origins::MC);
	isDead = false;

	currentClipInfo = clipInfos[0];
}

void AnimPlayer::Update(float dt)
{
	SpriteGo::Update(dt);

	switch (sceneGame->GetStatus())
	{
	case GameStatus::Awake:
		UpdateAwake(dt);
		break;
	case GameStatus::Game:
		UpdateGame(dt);
		break;
	case GameStatus::GameOver:
		UpdateGameover(dt);
		break;
	case GameStatus::Pause:
		UpdatePause(dt);
		break;
	}

}

void AnimPlayer::UpdateAwake(float dt)
{
}

void AnimPlayer::UpdateGame(float dt)
{
	animator.Update(dt);
	shootTimer += dt;

	if (InputManager::GetKey(sf::Keyboard::LControl))
	{
		Shoot();
	}

	direction.x = InputManager::GetAxisRaw(Axis::Horizontal);
	direction.y = InputManager::GetAxisRaw(Axis::Vertical);

	float mag = Utils::MyMath::Magnitude(direction);
	if (mag > 1.f)
	{
		direction /= mag;
	}

	Translate(direction * speed * dt);

	if (direction.x != 0.f || direction.y > 0.f)
	{
		currentClipInfo = clipInfos[1];
	}
	else
	{
		currentClipInfo = clipInfos[0];
	}

	const std::string& clipId = (direction.x != 0.f || direction.y != 0.f) ?
		currentClipInfo.move : currentClipInfo.idle;

	if (animator.GetCurrentClipId() != clipId)
	{
		animator.Play(clipId);
	}

	// «√∑π¿ÃæÓ ∏  ¿Ã≈ª √º≈©
	if (position.x < -270) position.x = -270.f;
	if (position.x > 270) position.x = 270.f;
	if (position.y < -480) position.y = -480.f;
	if (position.y > 480) position.y = 480.f;

}

void AnimPlayer::UpdateGameover(float dt)
{
}

void AnimPlayer::UpdatePause(float dt)
{
}

void AnimPlayer::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void AnimPlayer::Shoot()
{
	if (shootTimer >= shootInterval)
	{
		shootTimer = 0.f;
		Bullet* bullet = new Bullet();
		bullet->Init();
		bullet->Reset();
		bullet->SetPosition(position);
		sceneGame->AddGameObject(bullet);
	}
}


void AnimPlayer::OnDie()
{
	SetActive(false);
	sceneGame->RemoveGameObject(this);
	sceneGame->SetStatus(GameStatus::GameOver);
}

