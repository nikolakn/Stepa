#ifndef NKUNITRENDER_H
#define NKUNITRENDER_H

#include "include/framework.h"
#include "nkmodel.h"
#include "game/nkunits.h"

class NkUnitRender : public NkModel
{
public:
    NkUnitRender();
    ~NkUnitRender();
    // NkModel interface
public:
    int init();
    int init(float w, float h);
    void render(mat4 *ProjectionMatrix, mat4 *mModelView);
    void releaseScene();
    GLuint createBuffer();
    void loadShaders();
    void setXY(int xS, int yS);
private:
    float x,y;        /**<coordinates of selected hex */
    GLuint selvao;
    GLuint position_vbo;
    GLint mvp_mat_loc;
    GLint view_mat_loc;
    GLint gbuffer_instanced_pos;
    GLuint vertex_vbo;
    GLuint tex_vbo;
    GLuint tex;
    GLuint programsel;
    GLuint index_vbo;
    NkUnits units;

};

#endif // NKUNITRENDER_H
