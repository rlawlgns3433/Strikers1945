#include "pch.h"
#include "PlayerTable.h"
#include "rapidcsv.h"

PlayerData PlayerTable::Undefined = PlayerData();

PlayerTable::PlayerTable(Types type)
	: DataTable(type)
{
}

bool PlayerTable::Load()
{
	rapidcsv::Document doc("tables/PlayerTable.csv");
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		auto row = doc.GetRow<std::string>(i);
		PlayerType id = (PlayerType)std::stoi(row[0]);

		if (table.find(id) != table.end())
		{
			std::cout << "Player ID Duplicated" << std::endl;
			return false;
		}
		
		table[id] = PlayerData();
		table[id].id = id;
		table[id].animationIdleClipId = row[1];
		table[id].animationMoveClipId = row[2];
		table[id].animationDeadClipId = row[3];
		table[id].animationBombClipId = row[4];
		table[id].shootInterval = std::stof(row[5]);
		table[id].invincibleInterval = std::stof(row[6]);
		table[id].maxHp = std::stoi(row[7]);
		table[id].speed = std::stoi(row[8]);
		table[id].score = std::stoi(row[9]);
		table[id].damage = std::stoi(row[10]);
		table[id].lifes = std::stoi(row[11]);
		table[id].bombCount = std::stoi(row[12]);
		table[id].maxPowerLevel = std::stoi(row[13]);
		table[id].initialpowerLevel = std::stoi(row[14]);
		table[id].maxHelperCount = std::stoi(row[15]);
		table[id].initialpowerLevel = std::stoi(row[16]);
		
	}

	return true;
}

void PlayerTable::Release()
{
	table.clear();
}

const PlayerData PlayerTable::Get(PlayerType id)
{
	auto find = table.find(id);

	if (find == table.end())
	{
		std::cout << "";
		return Undefined;
	}

	return find->second;
}