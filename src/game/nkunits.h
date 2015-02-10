#ifndef NKUNITS_H
#define NKUNITS_H

#include <vector>
#include <map>
#include <string>
#include <iostream>

using namespace std;

struct Unit
{
    int x;
    int y;
    int type;
};

class NkUnitsPosition : public std::map<int,Unit>
{
public:
    NkUnitsPosition();
    void addUnit(int x, int y, int type);
private:

};

#endif // NKUNITS_H
