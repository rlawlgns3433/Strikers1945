#include "pch.h"
#include "AnimPlayer.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "UiHUD.h"
#include "Item.h"
#include "PlayerHelper.h"

AnimPlayer::AnimPlayer(const std::string& name)
	: SpriteGo(name)
{
}

void AnimPlayer::Init()
{
	SpriteGo::Init();


	animator.SetTarget(&sprite);
	SetScale({ 2.f,2.f });

	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", "animation/Player/Bomb.csv", false, false });
	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", "animation/Player/Bomb.csv", true, false });
	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", "animation/Player/Bomb.csv", false, true });

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
	hud = new UiHUD();
	hud->Init();
	hud->Reset();
	sceneGame->AddGameObject(hud, Layers::Ui);

	std::function<void()> deadEvent = std::bind(&AnimPlayer::DeadEvent, this);
	animator.AddEvent("animation/Player/Dead.csv", 10, deadEvent);
	 
	std::function<void()> bombEvent = std::bind(&AnimPlayer::BombEvent, this);
	animator.AddEvent("animation/Player/Bomb.csv", 46, bombEvent);

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
	animator.Play("animation/Player/Idle.csv");

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
	powerLevel = 1;
	hp = maxHp;
	lifes = 3;
	bombCount = 2;
	damage = 75;
	score = 0;
	currentHelperCount = powerLevel;

	for (int i = 0; i < 4; ++i)
	{
		// 플레이어 헬퍼 4개 생성
		PlayerHelper* helper = new PlayerHelper();
		helper->Init();
		helper->Reset();
		helper->SetPosition(position + playerHelpersOffset[i]);
		helper->SetOffset(playerHelpersOffset[i]);
		
		// 플레이어 헬퍼 4개 모두 비활성화
		helper->SetActive(false);
		playerHelpers.push_back(helper);
		// 플레이어 헬퍼 카운트에 따라 SetActive(true)
		sceneGame->AddGameObject(helper);
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
	std::cout << isInvincible << std::endl;
	 

	animator.Update(dt);
	shootTimer += dt;

	if (InputManager::GetKey(sf::Keyboard::LControl) && !isDead)
	{
		Shoot();
	}

	if (InputManager::GetKeyDown(sf::Keyboard::LShift) && !isDead)
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

	const std::string& clipId = isDead ? currentClipInfo.dead : isBomb ? currentClipInfo.bomb : (direction.x != 0.f || direction.y != 0.f) ?
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

		if (!isBomb)
		{
			if (invincibleTimer > invincibleInterval

			{
				isInvincible = false;
				invincibleTimer = 0.f;
				isDead = false;
			}
		}
		else
		{
			if (invincibleTimer > 4.f)
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
	if (bombCount <= 0) return;

	isBomb = true;
	hud->SetBombCount(--bombCount);

	std::cout << animator.GetCurrentClipId() << std::endl;
	// isActive가 true인 모든 적에게 데미지 
	const std::list<Enemy*>& enemyList = sceneGame->GetEnemyList();

	for(auto& enemy : enemyList)
	{
		enemy->OnDamage(1000);
	}
}


void AnimPlayer::OnDie()
{
	// 죽었을 때 애니메이션 재생
	isDead = true;

	if (powerLevel > 1)
	{
		powerLevel = 1;
		currentHelperCount = 1;

		Item* item = Item::Create(Item::Types::PowerUp);
		item->Init();
		item->Reset();
		item->SetPosition(position + sf::Vector2f(-50.f, -50.f));
		sceneGame->ItemList.push_back(item);
		sceneGame->AddGameObject(item);
	}
}

void AnimPlayer::DeadEvent()
{
	hud->SetLifes(--lifes);
	// 애니메이션 끝날 때 이곳을 호출
	// 플레이어 사망 프레임이 종료되었을 때 실행
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
} 

void AnimPlayer::AddPowerLevel(int add)
{
	powerLevel += add;

	if (powerLevel > maxPowerLevel)
	{
		powerLevel = maxPowerLevel;
	}
	damage = 100 + 50 * (powerLevel - 1);

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

void AnimPlayer::SetPowerLevel(int powerLevel)
{
	this->powerLevel = powerLevel;

	if (powerLevel > maxPowerLevel)
	{
		powerLevel = maxPowerLevel;
	}
	damage = 100 + 50 * (powerLevel - 1);

	currentHelperCount = powerLevel;
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
