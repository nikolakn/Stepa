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

#ifndef NKMODEL_H
#define NKMODEL_H

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/framework.h"
/**
 * @brief The NkModel class
 * Abstract class for models
 */
class NkModel
{
public:
    virtual	int init() = 0;
    virtual	int init(float w, float h) = 0;
    virtual void render(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView) = 0;
    virtual	void releaseScene() = 0;
    virtual GLuint createBuffer() = 0;
    virtual void loadShaders() = 0;
 protected:
    GLuint program;
    framework frm;
};

#endif // NKMODEL_H
