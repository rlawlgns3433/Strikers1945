#pragma once
#include "Singleton.h"
#include "DataTable.h"
#include "EnemyTable.h"
#include "ItemTable.h"

class DataTableManager : public Singleton<DataTableManager>
{
	friend Singleton<DataTableManager>;
protected :

	DataTableManager(const DataTableManager&)				 = delete;
	DataTableManager(DataTableManager&&)					 = delete;
	DataTableManager& operator=(const DataTableManager&)	 = delete;
	DataTableManager& operator=(DataTableManager&&)			 = delete;

	DataTableManager();
	~DataTableManager();

	std::unordered_map<DataTable::Types, DataTable*> tables;

public :
	void Init();
	void Release();

	template <typename T>
	T* Get(DataTable::Types type);
};

template<typename T>
inline T* DataTableManager::Get(DataTable::Types type)
{
	auto find = tables.find(type);
	if (find == tables.end()) return nullptr;

	return dynamic_cast<T*>(find->second);
}

#define DATATABLE_MANAGER (DataTableManager::Instance())
//#define STRING_TABLE (DATATABLE_MANAGER.Get<StringTable>(DataTable::Types::String))
#define ENEMY_TABLE (DATATABLE_MANAGER.Get<EnemyTable>(DataTable::Types::Enemy))
#define ITEM_TABLE (DATATABLE_MANAGER.Get<ItemTable>(DataTable::Types::Item))
