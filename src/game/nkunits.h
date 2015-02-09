#ifndef NKUNITS_H
#define NKUNITS_H

#include <vector>
#include <string>

using namespace std;

struct Unit
{
    int x;
    int y;
    int type;
};

class NkUnits : public std::vector<Unit>
{
public:
    NkUnits();
    void addUnit(Unit unit);
    void addUnit(int x, int y, int type);
private:

};

#endif // NKUNITS_H
