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
}

void DataTableManager::Release()
{
	for (auto table : tables)
	{
		delete table.second;
	}
	tables.clear();
}
