#include "nkunits.h"

NkUnits::NkUnits()
{
}

void NkUnits::addUnit(Unit unit)
{
    push_back(unit);

}
void NkUnits::addUnit(int x , int y, int type)
{
    Unit n{x, y, type};
    push_back(n);

}
