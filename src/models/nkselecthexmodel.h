/*
* Stepa - OpenGL hex game
*
* (C) Copyright 2014
* Author: Nikola Knezevic <nkcodeplus@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; version 2
* of the License.
*/

/** @file nkselecthexmodel.h
 *  @brief selection Hex for map.
 *
 *  @author Nikola Knezevic (nkcodeplus@gmail.com)
 *  @bug No known bugs.
 */

#ifndef NKSELECTHEXMODEL_H
#define NKSELECTHEXMODEL_H
#include "include/framework.h"
#include "nkmodel.h"

/**
 * @brief The NkSelectHexModel class
 * draw selection mark
 */
class NkSelectHexModel : public NkModel
{
public:
    NkSelectHexModel();
    /**
     * @brief init load and bind data
     */
    void init();
    void render(mat4 *ProjectionMatrix, mat4 *mModelView);
    void releaseScene();

    /**
     * @brief createHex bind buffers
     * @return vao
     */
    GLuint createHex();

    /**
     * @brief LoadShaders
     * load shaders from string
     */
    void loadShaders();
    /**
     * @brief setXY
     * set selected hex
     */
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
    vector<vec4> positions;
    GLuint tex;
    GLuint programsel;
    GLuint index_vbo;

};

#endif // NKSELECTHEXMODEL_H
