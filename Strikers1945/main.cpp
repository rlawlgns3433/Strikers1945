#include "pch.h"

int main()
{
    FRAMEWORK.Init(540, 960, "GameTitle");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}