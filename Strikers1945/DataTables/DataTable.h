#pragma once
class DataTable
{
public :
	enum class Types // Data Table�� ����
	{
		String, 
		Enemy,
		Item
	};
	static const int TotalTypes = 2;

private :
	Types type;

	DataTable(const DataTable&)				 = delete;
	DataTable(DataTable&&)					 = delete;
	DataTable& operator=(const DataTable&)   = delete;
	DataTable& operator=(DataTable&&)		 = delete;

public :
	DataTable(Types type) : type(type) {};
	virtual ~DataTable()  = default;

	virtual bool Load() = 0;
	virtual void Release() = 0;
};