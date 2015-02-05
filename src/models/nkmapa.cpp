#include "nkmapa.h"

NkMapa::NkMapa(int Size)
{
    size = Size;
    float x=0;
    float y=0;
    float mov=2;
    float scale=2.5;
    float pos= mov*scale;
    for (int n=1; n <= size; n++){
        std::ostringstream stream;
        stream << "./data/maps/mapa3/mapa2_" << n <<".png";
        NkMapTexture *m = new NkMapTexture(x,y,scale,stream.str());
        x=x+pos;
        if(n%8==0){
            x=0;
            y=y+pos;
        }

        m->init();
        map.push_back(m);
    }
}

NkMapa::~NkMapa()
{
    for(auto m : map){
        delete m;
    }
}

void NkMapa::render(mat4 *ProjectionMatrix, mat4 *mModelView)
{
    for(auto m : map){
        m->render(ProjectionMatrix,mModelView);
    }
}
