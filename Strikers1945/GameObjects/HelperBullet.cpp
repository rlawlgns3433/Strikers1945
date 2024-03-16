#include "pch.h"
#include "HelperBullet.h"
#include "SceneGame.h"

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
	isDeterminedTarget = false;
	angle = Utils::MyMath::Angle(direction);
	SetTexture("graphics/Strikers1945/helperBullet.png");
	SetOrigin(Origins::BC);
}

void HelperBullet::Update(float dt)
{
	SpriteGo::Update(dt);
	
	if (!isDeterminedTarget)
	{
		closestEnemy = FindClosestEnemy();
		if(closestEnemy != nullptr)
			isDeterminedTarget = true;
		else
		{
			isDeterminedTarget = false;
			return;
		}
	}
	if(isDeterminedTarget && closestEnemy->GetActive() 
		 && !closestEnemy->isDead())
	{
		Utils::MyMath::Normalize(direction = closestEnemy->GetPosition() - position);

		angle = Utils::MyMath::Angle(direction);
		SetRotation(angle + 90);
	}
	else
	{
		isDeterminedTarget = false;
	}
	if (abs(position.x) > 300 || abs(position.y) > 500)
	{
		SetActive(false);
	}

	Translate(direction * speed * dt);
}
 
void HelperBullet::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
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
