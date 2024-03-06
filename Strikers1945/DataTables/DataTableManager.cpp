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
	
	// String Table ·Îµå
	DataTable* table = new StringTable(DataTable::Types::String);
	table->Load();
	tables.insert({ DataTable::Types::String, table});

}

void DataTableManager::Release()
{
	for (auto table : tables)
	{
		delete table.second;
	}
	tables.clear();
}
