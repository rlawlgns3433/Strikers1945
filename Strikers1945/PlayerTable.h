#pragma once
#include "DataTable.h"
#include "AnimPlayer.h"

struct PlayerData
{
	AnimPlayer::Type id;

	std::string nameId;

	std::string animationIdleClipId;
	std::string animationMoveClipId;
	std::string animationDeadClipId;
	std::string animationBombClipId;

	float shootInterval;
	float invincibleInterval;

	int maxHp;
	int speed;
	int score;
	int damage;
	int lifes;
	int bombCount;
	int maxPowerLevel;
	int initialpowerLevel;
	int maxHelperCount;
	int initialHelperCount;
};

class PlayerTable : public DataTable
{
public:
	static PlayerData Undefined;

protected:
	PlayerTable(const PlayerTable&) = delete;
	PlayerTable(PlayerTable&&) = delete;
	PlayerTable& operator=(const PlayerTable&) = delete;
	PlayerTable& operator=(PlayerTable&&) = delete;

	std::unordered_map <AnimPlayer::Type, PlayerData> table;

public:
	PlayerTable(Types type);
	~PlayerTable() override = default;

	bool Load() override;
	void Release() override;

	const PlayerData Get(AnimPlayer::Type id);
};