#ifndef NKUNITS_H
#define NKUNITS_H

#include <vector>
#include <string>

using namespace std;

struct Unit
{
    int x;
    int y;
    int position;
};

class NkUnits
{
public:
    NkUnits();
    void addUnit(Unit unit);
private:
    vector<Unit> units;
};

#endif // NKUNITS_H
