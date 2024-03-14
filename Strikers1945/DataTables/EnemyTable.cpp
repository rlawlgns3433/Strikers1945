#include "pch.h"
#include "EnemyTable.h"
#include "rapidcsv.h"

EnemyData EnemyTable::Undefined = EnemyData();

EnemyTable::EnemyTable(Types type)
    : DataTable(type)
{
}

bool EnemyTable::Load()
{
    Release();

    rapidcsv::Document doc("tables/EnemyTable.csv", rapidcsv::LabelParams());
    for (int i = 0; i < doc.GetRowCount(); ++i)
    {
        auto row = doc.GetRow<std::string>(i);
        Enemy::Types id = (Enemy::Types)std::stoi(row[0]);

        if (table.find(id) != table.end())
        {
            std::cout << "Enmey ID Duplicated" << std::endl;
            return false;
        }

        table[id] = EnemyData();
        table[id].id = id;
        table[id].nameId = row[1];
        table[id].animationMoveClipId = row[2];
        table[id].animationDeadClipId = row[3];
        table[id].maxHp = std::stoi(row[4]);
        table[id].speed = std::stoi(row[5]);
        table[id].score = std::stoi(row[6]);
        table[id].attackInterval = std::stof(row[7]);
        table[id].shootType = (Enemy::ShootTypes)std::stoi(row[8]);
    }
    
    return true;
}

void EnemyTable::Release()
{
    table.clear();
}

const EnemyData EnemyTable::Get(Enemy::Types id)
{
    auto find = table.find(id);
    if (find == table.end())
    {
        std::cout << "";
        return Undefined;

    }
    return find->second;
}