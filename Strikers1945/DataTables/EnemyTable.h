#pragma once
#include "DataTable.h"
#include "Enemy.h"

struct EnemyData
{
	Enemy::Types id;
	std::string nameId;
	std::string animationMoveClipId;
	std::string animationDeadClipId;

	int maxHp;
	int speed;
	int score;

	float attackInterval;

	Enemy::ShootTypes shootType;
};

class EnemyTable : public DataTable
{
public:
	static EnemyData Undefined;

protected:
	EnemyTable(const EnemyTable&) = delete;
	EnemyTable(EnemyTable&&) = delete;
	EnemyTable& operator=(const EnemyTable&) = delete; 
	EnemyTable& operator=(EnemyTable&&) = delete;
	
	std::unordered_map <Enemy::Types, EnemyData> table;

public:
	EnemyTable(Types type);
	~EnemyTable() override = default;

	bool Load() override;
	void Release() override;

	const EnemyData Get(Enemy::Types id);
};