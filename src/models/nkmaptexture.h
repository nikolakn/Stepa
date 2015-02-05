#ifndef NKMAPTEXTURE_H
#define NKMAPTEXTURE_H

#include "include/framework.h"
#include "nkmodel.h"

class NkMapTexture : public NkModel
{
public:
    NkMapTexture(float x, float y, float scale, string filenameString);
    ~NkMapTexture();
    int init();
    int init(float w, float h);
    void render(mat4 *ProjectionMatrix, mat4 *mModelView);
    void releaseScene();
    GLuint createBuffer();
    void loadShaders();
private:
    NkMapTexture();
    float x,y,scale;        /**<coordinates of texture */
    string fileName;
    GLuint selvao;
    GLuint position_vbo;
    GLint mvp_mat_loc;
    GLint view_mat_loc;
    GLint gbuffer_instanced_pos;
    GLuint vertex_vbo;
    GLuint tex_vbo;
    vector<vec4> positions;
    GLuint tex;
    GLuint programsel;
    GLuint index_vbo;
};

#endif // NKMAPTEXTURE_H
