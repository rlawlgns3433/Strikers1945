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

	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", false, false });
	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", true, false });
	clipInfos.push_back({ "animation/Player/Idle.csv", "animation/Player/Move.csv", "animation/Player/Dead.csv", false, true });

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetScene(SceneIDs::SceneGame));
	hud = new UiHUD();
	hud->Init();
	hud->Reset();
	sceneGame->AddGameObject(hud, Layers::Ui);

	std::function<void()> deadEvent = std::bind(&AnimPlayer::DeadEvent, this);
	animator.AddEvent("animation/Player/Dead.csv", 10, deadEvent);


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
	currentHelperCount = 4;

	for (int i = 0; i < 4; ++i)
	{
		// �÷��̾� ���� 4�� ����
		PlayerHelper* helper = new PlayerHelper();
		helper->Init();
		helper->Reset();
		helper->SetPosition(position + playerHelpersOffset[i]);
		
		// �÷��̾� ���� 4�� ��� ��Ȱ��ȭ
		//helper->SetActive(false);
		playerHelpers.push_back(helper);
		// �÷��̾� ���� ī��Ʈ�� ���� SetActive(true)
		sceneGame->AddGameObject(helper);
	}

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
	animator.Update(dt);
	shootTimer += dt;

	if (InputManager::GetKey(sf::Keyboard::LControl) && !isDead)
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

		// �÷��̾� �� ��Ż üũ
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
		}
		else if (invincibleTimer > invincibleInterval - 1.f)
		{
			isDead = false;
			SetPosition({ 0, 460 });
			SetActive(true);
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


void AnimPlayer::OnDie()
{
	// �׾��� �� �ִϸ��̼� ���
	isDead = true;
	isInvincible = true;
	if (powerLevel > 1)
	{
		powerLevel = 1;
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
	// �ִϸ��̼� ���� �� �̰��� ȣ��
	// �÷��̾� ��� �������� ����Ǿ��� �� ����
	if (lifes <= 0)
	{
		SetActive(false);
		sceneGame->SetStatus(GameStatus::GameOver);
		return;
	}
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
}

void AnimPlayer::SetPowerLevel(int powerLevel)
{
	this->powerLevel = powerLevel;

	if (powerLevel > maxPowerLevel)
	{
		powerLevel = maxPowerLevel;
	}
	damage = 100 + 50 * (powerLevel - 1);

	currentHelperCount = powerLevel - 1;
	if (currentHelperCount < 0)
	{
		currentHelperCount = 0;
	}
	else if (currentHelperCount > 4)
	{
		currentHelperCount = 4;
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
		hud->SetLifes(lifes);
	}
	isCheated = !isCheated;
}