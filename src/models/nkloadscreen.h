#ifndef NKLOADSCREEN_H
#define NKLOADSCREEN_H
#include "include/framework.h"
#include "nkmodel.h"

class NkLoadscreen : public NkModel
{
public:
    NkLoadscreen();
    ~NkLoadscreen();

    // NkModel interface
public:
    void render(mat4 *ProjectionMatrix, mat4 *mModelView);
    void releaseScene();
    GLuint createBuffer();
    void loadShaders();
    int init();
    int init(float w, float h);

private:
    GLuint selvao;
    GLint view_mat_loc;
    GLuint index_vbo;
    GLuint vertex_vbo;
    float m_duzina;
    float m_visina;
};

#endif // NKLOADSCREEN_H
