#include "pch.h"
#include "AnimPlayer.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "UiHUD.h"

AnimPlayer::AnimPlayer(const std::string& name)
	: SpriteGo(name)
{
}

void AnimPlayer::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);
	SetScale({ 2.f,2.f });

	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", false, false });
	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", true, false });
	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", false, true });

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
	hud = new UiHUD();
	hud->Init();
	hud->Reset();
	sceneGame->AddGameObject(hud, Layers::Ui);
	SetPosition({ 0, 450.f });

	std::function<void()> deadEvent = std::bind(&AnimPlayer::DeadEvent, this);
	animator.AddEvent("animation/Player/Dead.csv", 9, deadEvent);
}

void AnimPlayer::Reset()
{
	animator.Play("animation/Player/Idle.csv");

	SetOrigin(Origins::MC);
	isDead = false;
	currentClipInfo = clipInfos[0];

	hud->SetBombCount(bombCount);
	hud->SetLifes(lifes);
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

	if (!isDead)
	{
		Translate(direction * speed * dt);
	}

	if (isDead)
	{
		currentClipInfo = clipInfos[2];
	}
	else if (direction.x != 0.f || direction.y > 0.f)
	{
		currentClipInfo = clipInfos[1];
	}
	else
	{
		currentClipInfo = clipInfos[0];
	}

	const std::string& clipId = isDead ? currentClipInfo.dead : (direction.x != 0.f || direction.y != 0.f) ?
		currentClipInfo.move : currentClipInfo.idle;

	if (animator.GetCurrentClipId() != clipId)
	{
		animator.Play(clipId);
	}

		// 플레이어 맵 이탈 체크
	if (position.x < -270) position.x = -270.f;
	if (position.x > 270) position.x = 270.f;
	if (position.y < -480) position.y = -480.f;
	if (position.y > 480) position.y = 480.f;

	if (isInvincible)
	{
		invincibleTimer += dt;

		if (invincibleTimer > invincibleInterval)
		{
			isInvincible = false;
			invincibleTimer = 0.f;
			isDead = false;
		}
	}
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
	window.draw(shape);
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
	// 죽었을 때 애니메이션 재생
	isDead = true;
	isInvincible = true;
}

void AnimPlayer::DeadEvent()
{
	hud->SetLifes(--lifes);
	// 애니메이션 끝날 때 이곳을 호출
	// 플레이어 사망 프레임이 종료되었을 때 실행
	if (lifes <= 0)
	{
		SetActive(false);
		sceneGame->RemoveGameObject(this);
		sceneGame->SetStatus(GameStatus::GameOver);
		return;
	}
}