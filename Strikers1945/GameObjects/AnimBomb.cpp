#include "pch.h"
#include "AnimBomb.h"
#include "SceneGame.h"
#include "EnemyProjectile.h"

AnimBomb::AnimBomb(const std::string& name)
	: SpriteGo(name)
{
}

void AnimBomb::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	std::function<void()> bombTerminateEvent = std::bind(&AnimBomb::BombTerminateEvent, this);
	animator.AddEvent(animationClipId, 15, bombTerminateEvent);

	std::function<void()> bombKillEnemyEvent = std::bind(&AnimBomb::BombKillEnemyEvent, this);
	animator.AddEvent(animationClipId, 1, bombKillEnemyEvent);
}

void AnimBomb::Reset()
{
	SetOrigin(Origins::MC);
	SetPosition({ 0,0 });
	animator.Play(animationClipId);
}

void AnimBomb::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);
}

void AnimBomb::BombTerminateEvent()
{
	SetActive(false);
}

void AnimBomb::BombKillEnemyEvent()
{
	const std::list<Enemy*>& enemyList = sceneGame->GetEnemyList();

	for (auto& enemy : enemyList)
	{
		enemy->OnDamage(1000);
	}

	const std::list<EnemyProjectile*>& usingProjectileList = sceneGame->GetUsingProjectileList();
	std::list<EnemyProjectile*>& unusingProjectileList = sceneGame->GetUnusingProjectileList();
	
	for (auto& projectile : usingProjectileList)
	{
		projectile->SetActive(false);
		unusingProjectileList.push_back(projectile);
	}
}