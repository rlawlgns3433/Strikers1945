#include "pch.h"

DataTableManager::DataTableManager()
{
}

DataTableManager::~DataTableManager()
{
}

void DataTableManager::Init()
{
	Release();
	
	DataTable* enemyTable = new EnemyTable(DataTable::Types::Enemy);
	enemyTable->Load();
	tables.insert({ DataTable::Types::Enemy, enemyTable });

	DataTable* itemTable = new ItemTable(DataTable::Types::Item);
	itemTable->Load();
	tables.insert({ DataTable::Types::Item, itemTable });
}

void DataTableManager::Release()
{
	for (auto table : tables)
	{
		delete table.second;
	}
	tables.clear();
}
