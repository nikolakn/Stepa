#ifndef NKBLECKRECT_H
#define NKBLECKRECT_H
#include "include/framework.h"
#include "nkmodel.h"

class NkBleckRect : public NkModel
{
public:
    NkBleckRect();
    ~NkBleckRect();
    int init();
    void render(mat4 *ProjectionMatrix, mat4 *mModelView);
    void render(float x, float y, float w, float h);
    void releaseScene();
    GLuint createBuffer();
    void loadShaders();
    int init(float w, float h);

    int duzina() const;
    void setDuzina(int duzina);

    int visina() const;
    void setVisina(int visina);

private:
    GLuint selvao;
    GLint view_mat_loc;
    GLuint index_vbo;
    GLuint vertex_vbo;
    float m_duzina;
    float m_visina;
};

#endif // NKBLECKRECT_H
