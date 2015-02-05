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

/** @file nkhex2.h
 *  @brief Hex grid for game map.
 *
 * instanced rendering hex poligon
 *
 *  @author Nikola Knezevic (nkcodeplus@gmail.com)
 *  @bug No known bugs.
 */

#ifndef NKHEX2_H
#define NKHEX2_H
#include "include/framework.h"
#include "nkmodel.h"

/**
 * @brief The NkHex2 class Hex grid for game map.
 */
class NkHex2 : public NkModel
{
public:
    NkHex2();
    ~NkHex2();
    /**
     * @brief init load and bind data
     */
    int init();

    /**
     * @brief renderSel  hex map without texture. use shader programsel.
     *
     * Draw hex map for Picking technique.
     * Every hex is draw on it own color.
     * Red color is x coordinate (0-255)
     * Green color is y coordinate (0-255)
     * it gives 65536 combination and this is max number of different hex
     * if needed more hex, technique can be use on blue color,
     * but shader programsel must be changed.
     *
     * @param ProjectionMatrix
     * @param mModelView
     */
    void renderSel(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView);

    /**
     * @brief render  hex map with texture and program1
     *
     * instanced rendering of hex map
     *
     * @param ProjectionMatrix
     * @param mModelView
     */
    void render(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView);

    void releaseScene();

    /**
     * @brief createHex bind buffers
     * @return vao
     */
    GLuint createBuffer();

    /**
     * @brief LoadShaders
     * load shaders from string with constants in compile time
     */
    void loadShaders();
    int init(float w, float h);
    void renderLine(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView);
private:
    const float  HEX_WIDTH=0.2;  	/**<width of one hex */
    const float  HEX_HEIGHT=0.18; 	/**<height of one hex*/
    /***
     *number of hex in map
     *for selection to work nuber of hex<=65536
     *for more hex: shader program programsel need to be changed to include blue color too
     **/
    const int HEX_SIZE = 32886;
    const int HEX_LINE_NUM = 174;   /**<number of hex in one line*/

    GLuint box;
    GLint gbuffer_instanced_mvp_mat_loc;
    GLint gbuffer_instanced_normal_mat_loc;
    GLint gbuffer_instanced_view;
    GLint gbuffer_instanced_pos;

    GLint gbuffer_instanced_mvp_mat_loc_sel;
    GLint gbuffer_instanced_normal_mat_loc_sel;
    GLint gbuffer_instanced_view_sel;
    GLint gbuffer_instanced_pos_sel;
    GLuint vertex_vbo;
    GLuint normal_vbo;
    GLuint tex_vbo=0;
    vector<vec4> positions;
    GLuint tex;
    GLuint index_vbo;
    GLuint programsel;
    GLuint programline;

};

#endif // NKHEX2_H
