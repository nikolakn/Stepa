#ifndef NKBLECKRECT_H
#define NKBLECKRECT_H
#include "include/framework.h"
#include "nkmodel.h"

class NkBleckRect : public NkModel
{
public:
    NkBleckRect();
    void init();
    void render(mat4 *ProjectionMatrix, mat4 *mModelView);
    void render(int x, int y, int w, int h);
    void releaseScene();
    GLuint createHex();
    void loadShaders();
    void init(int w, int h);
private:
    float x,y;        /**<coordinates of selected hex */
    GLuint selvao;
    GLuint position_vbo;
    GLint mvp_mat_loc;
    GLint view_mat_loc;
    GLint gbuffer_instanced_pos;

    vector<vec4> positions;
    GLuint tex;
    GLuint programsel;
    GLuint index_vbo;

    int m_duzina;
    int m_visina;
};

#endif // NKBLECKRECT_H
