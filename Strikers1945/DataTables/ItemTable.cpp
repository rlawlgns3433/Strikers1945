#include "pch.h"
#include "ItemTable.h"
#include "rapidcsv.h"

ItemData ItemTable::Undefined = ItemData();

ItemTable::ItemTable(Types type)
	: DataTable(type)
{
}

bool ItemTable::Load()
{
	Release();

	rapidcsv::Document doc("tables/ItemTable.csv");
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		auto row = doc.GetRow<std::string>(i);
		Item::Types id = (Item::Types)std::stoi(row[0]);

		if (table.find(id) != table.end())
		{
			std::cout << "Item ID Duplicated" << std::endl;
			return false;
		}

		table[id] = ItemData();
		table[id].id = id;
		table[id].nameId = row[1];
		table[id].animationClipId = row[2];
		table[id].speed = std::stof(row[3]);
		table[id].movement = (Item::MovementType)std::stoi(row[4]);
	}

	return true;
}

void ItemTable::Release()
{
	table.clear();
}

const ItemData ItemTable::Get(Item::Types id)
{
	auto find = table.find(id);
	if (find == table.end())
	{
		return Undefined;
	}

	return find->second;
}