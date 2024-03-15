#include "pch.h"
#include "AnimPlayer.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "UiHUD.h"
#include "Item.h"
#include "PlayerHelper.h"
#include "AnimBomb.h"

AnimPlayer::AnimPlayer(PlayerType playerType, const std::string& name)
	: SpriteGo(name)
{
	windowSize = FRAMEWORK.GetWindowSize();
	data = PLAYER_TABLE->Get(playerType);
}

void AnimPlayer::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);

	SetScale({ 2.f,2.f });
	clipInfos.push_back({ data.animationIdleClipId, data.animationMoveClipId, data.animationDeadClipId, data.animationBombClipId, false, false });
	clipInfos.push_back({ data.animationIdleClipId, data.animationMoveClipId, data.animationDeadClipId, data.animationBombClipId, true, false });
	clipInfos.push_back({ data.animationIdleClipId, data.animationMoveClipId, data.animationDeadClipId, data.animationBombClipId, false, true });

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
	hud = new UiHUD();
	sceneGame->AddGameObject(hud, Layers::Ui);

	std::function<void()> deadEvent = std::bind(&AnimPlayer::DeadEvent, this);
	animator.AddEvent(data.animationDeadClipId, 10, deadEvent);
	 
	std::function<void()> bombEvent = std::bind(&AnimPlayer::BombEvent, this);
	animator.AddEvent(data.animationBombClipId, 46, bombEvent);

	playerHelpersOffset =
	{
		{-40.f, 0},
		{40.f, 0},
		{-80.f, 0},
		{80.f, 0}
	};
}

void AnimPlayer::Reset()
{
	SetPosition({ 0, 450.f });
	animator.Play(data.animationIdleClipId);

	for (auto& bullet : unusingBulletlist)
	{
		bullet->SetActive(false);
	}

	for (auto& bullet : usingBulletlist)
	{
		bullet->SetActive(false);
	}

	SetOrigin(Origins::MC);
	currentClipInfo = clipInfos[0];
	isDead = false;
	isInvincible = true;
	isCheated = false;
	powerLevel = data.initialpowerLevel;
	lifes = data.lifes;
	bombCount = data.bombCount;
	damage = data.damage;
	score = data.score;
	currentHelperCount = powerLevel;

	for (int i = 0; i < 4; ++i)
	{
		playerHelpers.push_back(new PlayerHelper());
		playerHelpers[i]->Init();
		playerHelpers[i]->Reset();
		playerHelpers[i]->SetPosition(position + playerHelpersOffset[i]);
		playerHelpers[i]->SetOffset(playerHelpersOffset[i]);
		playerHelpers[i]->SetActive(false);
		sceneGame->AddGameObject(playerHelpers[i]);
	}

	playerHelpers[0]->SetActive(true);

	hud->SetScore(score);
	hud->SetBombCount(bombCount);
	hud->SetLifes(lifes);
}

void AnimPlayer::Release()
{
	SpriteGo::Release();
}

void AnimPlayer::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);
	shootTimer += dt;

	if (InputManager::GetKey(sf::Keyboard::LControl))
	{
		Shoot();
	}

	if (InputManager::GetKeyDown(sf::Keyboard::LShift))
	{
		UseBomb();
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
	else if (!isDead && isBomb)
	{
		currentClipInfo = clipInfos[0];
	}
	else if (direction.x != 0.f || direction.y > 0.f)
	{
		currentClipInfo = clipInfos[1];
	}
	else
	{
		currentClipInfo = clipInfos[0];
	}

	const std::string& clipId = isDead ? currentClipInfo.dead :
		isBomb ? currentClipInfo.bomb :
		(direction.x != 0.f || direction.y != 0.f) ? currentClipInfo.move : currentClipInfo.idle;

	if (animator.GetCurrentClipId() != clipId)
	{
		animator.Play(clipId);
	}
	if (position.x < -270) position.x = -270.f;
	if (position.x > 270) position.x = 270.f;
	if (position.y < -480) position.y = -480.f;
	if (position.y > 480) position.y = 480.f;

	if (isInvincible)
	{
		invincibleTimer += dt;

		if (!isBomb)
		{
			if (invincibleTimer > invincibleInterval)

			{
				isInvincible = false;
				invincibleTimer = 0.f;
				isDead = false;
			}
		}
		else
		{
			if (invincibleTimer > 5.f)
			{
				isInvincible = false;
				invincibleTimer = 0.f;
				isBomb = false;
			}
		}
	}

	auto it = usingBulletlist.begin();
	while (it != usingBulletlist.end())
	{
		auto bullet = *it;
		if (!bullet->GetActive())
		{
			it = usingBulletlist.erase(it);
			unusingBulletlist.push_back(bullet);
		}
		else
		{
			++it;
		}
	}

}

void AnimPlayer::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void AnimPlayer::Shoot()
{
	if (isBomb || isDead) return;

	if (shootTimer >= shootInterval)
	{
		shootTimer = 0.f;

		Bullet* bullet = nullptr;
		if (unusingBulletlist.empty())
		{
			bullet = new Bullet();
			bullet->Init();
		}
		else
		{
			bullet = unusingBulletlist.front();
			unusingBulletlist.pop_front();
		}
		bullet->SetActive(true);
		bullet->Reset();
		bullet->SetPosition(position);

		usingBulletlist.push_back(bullet);
		sceneGame->AddGameObject(bullet);
	}
}

void AnimPlayer::UseBomb()
{
	if (isDead || bombCount <= 0) return;

	isBomb = true;
	isInvincible = true;
	hud->SetBombCount(--bombCount);
}


void AnimPlayer::OnDie()
{
	isDead = true;

	if (powerLevel > 1)
	{
		SetPowerLevel(1);
		SetHelperCount(1);

		Item* item = Item::Create(Item::Types::PowerUp);
		item->Init();
		item->Reset();
		item->SetPosition(position + sf::Vector2f(Utils::Random::GetRandomVector2(-100.f, -70.f)));
		sceneGame->ItemList.push_back(item);
		sceneGame->AddGameObject(item);
	}
}

void AnimPlayer::DeadEvent()
{
	hud->SetLifes(--lifes);
	if (lifes <= 0)
	{
		SetActive(false);
		sceneGame->SetStatus(GameStatus::GameOver);
		return;
	}
	SetPosition({ 0, 460 });
	isInvincible = true;
	isDead = false;

}

void AnimPlayer::BombEvent()
{
	isBomb = false;
	if (bombAnimator == nullptr)
	{
		bombAnimator = new AnimBomb();
		bombAnimator->Init();
		bombAnimator->Reset();
		sceneGame->AddGameObject(bombAnimator);
	}
	
	bombAnimator->SetActive(true);
	bombAnimator->Reset();
}

void AnimPlayer::AddPowerLevel(int add)
{
	powerLevel += add;

	if (powerLevel > maxPowerLevel)
	{
		powerLevel = maxPowerLevel;
	}
	damage = 100 + 50 * (powerLevel - 1);

	SetHelperCount(powerLevel);
}

void AnimPlayer::SetPowerLevel(int powerLevel)
{
	this->powerLevel = powerLevel;

	if (powerLevel > maxPowerLevel)
	{
		powerLevel = maxPowerLevel;
	}
	damage += 75 * (powerLevel - 1);

	SetHelperCount(powerLevel);
}

void AnimPlayer::SetCheatMode()
{
	if (isCheated)
	{
		SetLife(3);
		SetPowerLevel(1);
		hud->SetLifes(lifes);
	}
	else
	{
		SetLife(999);
		SetPowerLevel(99);
		SetBombItem(999);
		hud->SetLifes(lifes);
		hud->SetBombCount(bombCount );
	}
	isCheated = !isCheated;
}

void AnimPlayer::AddHelperCount(int add)
{
	currentHelperCount += add;
	if (currentHelperCount < 0)
	{
		currentHelperCount = 0;
	}
	else if (currentHelperCount > 4)
	{
		currentHelperCount = 4;
	}

	for (int i = 0; i < currentHelperCount; ++i)
	{
		playerHelpers[i]->SetActive(true);
	}
	for (int i = currentHelperCount; i < 4; ++i)
	{
		playerHelpers[i]->SetActive(false);
	}
}

void AnimPlayer::SetHelperCount(int currentHelperCount)
{
	this->currentHelperCount = currentHelperCount;

	if (this->currentHelperCount < 0)
	{
		this->currentHelperCount = 0;
	}
	else if (currentHelperCount > 4)
	{
		currentHelperCount = 4;
	}

	for (int i = 0; i < this->currentHelperCount; ++i)
	{
		playerHelpers[i]->SetActive(true);
	}
	for (int i = this->currentHelperCount; i < 4; ++i)
	{
		playerHelpers[i]->SetActive(false);
	}
}