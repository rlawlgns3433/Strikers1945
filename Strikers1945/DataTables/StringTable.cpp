#include "pch.h"
#include "rapidcsv.h"

std::string StringTable::Undefined = "Undefined ID";

StringTable::StringTable(Types type)
	: DataTable(type)
{
}

bool StringTable::Load()
{
	return Load(currentLanguage);
}

bool StringTable::Load(Languages language)
{
	static std::vector<std::string> filePathArray = { "Tables/StringTable_KR.csv" , "Tables/StringTable_EN.csv" };

	currentLanguage = language;
	table.clear();
	rapidcsv::Document doc(filePathArray[(int)currentLanguage], rapidcsv::LabelParams(0, -1));
	
	for (int i = 0; i < doc.GetRowCount(); ++i)
	{
		std::vector<std::string> idValues = doc.GetRow<std::string>(i);
		table.insert({ idValues[0], idValues[1]});
	}
	return true;
}

void StringTable::Release()
{
	table.clear();
}

const std::string& StringTable::Get(const std::string& id)
{
	auto find = table.find(id);
	if (find == table.end()) return Undefined;

	return find->second;
}
