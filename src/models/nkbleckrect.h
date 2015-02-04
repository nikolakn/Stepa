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
    void render(float x, float y, float w, float h);
    void releaseScene();
    GLuint createHex();
    void loadShaders();
    void init(int w, int h);

    int duzina() const;
    void setDuzina(int duzina);

    int visina() const;
    void setVisina(int visina);

private:
    GLuint selvao;
    GLint view_mat_loc;
    GLuint index_vbo;
    GLuint vertex_vbo;
    int m_duzina;
    int m_visina;
};

#endif // NKBLECKRECT_H
