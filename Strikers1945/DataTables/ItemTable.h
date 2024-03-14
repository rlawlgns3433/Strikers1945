#pragma once
#include "DataTable.h"
#include "Item.h"

struct ItemData
{
	Item::Types id;
	std::string nameId;
	std::string animationClipId;
	float speed;
	Item::MovementType movement;
};

class ItemTable : public DataTable
{
public:
	static ItemData Undefined;

protected:
	ItemTable(const ItemTable&) = delete;
	ItemTable(ItemTable&&) = delete;
	ItemTable& operator=(const ItemTable&) = delete;
	ItemTable& operator=(ItemTable&&) = delete;

	std::unordered_map <Item::Types, ItemData> table;

public:
	ItemTable(Types type);
	~ItemTable() override = default;

	bool Load() override;
	void Release() override;

	const ItemData Get(Item::Types id);
};