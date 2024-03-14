#include "pch.h"
#include "HelperBullet.h"

HelperBullet::HelperBullet(const std::string& name)
	: SpriteGo(name)
{
}

void HelperBullet::Init()
{
	SpriteGo::Init();
}

void HelperBullet::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	SetTexture("graphics/Strikers1945/helperBullet.png");
	SetOrigin(Origins::BC);
}

void HelperBullet::Update(float dt)
{
	SpriteGo::Update(dt);
	
	// ���� ����� ���� ã��
	if (!isDeterminedTarget)
	{
		closestEnemy = FindClosestEnemy();
		if(closestEnemy != nullptr)
			isDeterminedTarget = true;
	}
	// �� ���� ���� �ʾ��� �� ����
	if(isDeterminedTarget && closestEnemy->GetActive() && !closestEnemy->isDead())
	{
		Utils::MyMath::Normalize(direction = closestEnemy->GetPosition() - position);
	}
	else
	{
		isDeterminedTarget = false;
	}
	// �� ���� �׾��ٸ� �� ��ġ���� �ٽ� ����� �� ����
	// ȭ�� ���� ������ �ȴٸ� �ı�
	if (abs(position.x) > 300 || abs(position.y) > 500)
	{
		SetActive(false);
	}

	Translate(direction * speed * dt);
}
 
void HelperBullet::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
	// ���� �浹�Ͽ��ٸ� ����
	// ������ �������� ����
	if (closestEnemy == nullptr) return;

	if (GetActive() && GetGlobalBounds().intersects(closestEnemy->GetGlobalBounds()) &&
		Utils::MyMath::Distance(position, closestEnemy->GetPosition()) < 40.f)
	{
		closestEnemy->OnDamage(damage);
		SetActive(false);
	}
}

Enemy* HelperBullet::FindClosestEnemy()
{
	const std::list<Enemy*>& enemyList = sceneGame->GetEnemyList();

	if (enemyList.empty())
	{
		SetActive(false);
		return nullptr;
	}
	float enemyDist = FLT_MAX;
	std::pair<Enemy*, float> closestEnemy;
	for (auto& enemy : enemyList)
	{
		float curEnemyDist = Utils::MyMath::Distance(position, enemy->GetPosition());
		if (enemyDist > curEnemyDist)
		{
			closestEnemy.first = enemy;
			closestEnemy.second = curEnemyDist;
			enemyDist = curEnemyDist;
		}
	}
	return closestEnemy.first;
}
