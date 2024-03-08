#pragma once
#include "DataTable.h"
class StringTable : public DataTable
{
public :
	static std::string Undefined;

protected :
	StringTable(const StringTable&)				 = delete;
	StringTable(StringTable&&)					 = delete;
	StringTable& operator=(const StringTable&)   = delete;
	StringTable& operator=(StringTable&&)		 = delete;

	std::unordered_map < std::string, std::string> table;

	Languages currentLanguage = Languages::Korean;

public :
	StringTable(Types type);
	~StringTable() override						= default;

	bool Load() override;
	bool Load(Languages language);
	void Release() override;
	
	const std::string& Get(const std::string& id);
};