#ifndef NKMAPA_H
#define NKMAPA_H

#include <vector>
#include <string>
#include "include/framework.h"
#include "nkmaptexture.h"
using namespace std;

class NkMapa
{
public:
    NkMapa(int Size);
    ~NkMapa();
    void render(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView);
private:
    int size;
    vector<NkMapTexture*> map;
};

#endif // NKMAPA_H
