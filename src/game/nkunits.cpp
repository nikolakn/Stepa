#include "nkunits.h"

NkUnitsPosition::NkUnitsPosition()
{
}

void NkUnitsPosition::addUnit(int x , int y, int type)
{
    Unit un{x, y, type};
    int n=y*174+x;
    auto res = this->insert(std::make_pair(n,un));
    if ( ! res.second ) {
        cout << "key " <<  n << " already exists " << endl;
    }

}
