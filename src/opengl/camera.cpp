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

#include "camera.h"

NkCamera::NkCamera()
{
    position = glm::vec3(15,10,10);
    direction = glm::vec3(15, 0, 0);
    upRot = glm::vec3(0, 1, 0);
    setlookAt();
}

NkCamera::~NkCamera()
{

}

void NkCamera::setlookAt()
{
    modelView= glm::lookAt(position, direction,  upRot);
}

void NkCamera::setModelView(const glm::mat4 &mv)
{
    modelView = mv;
}

void NkCamera::rotate(float amount)
{
    modelView = glm::rotate(modelView, amount, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 NkCamera::ExtractCameraPos_NoScale(const glm::mat4 & a_modelView)
{
    glm::mat3 rotMat(a_modelView);
    glm::vec3 d(a_modelView[3]);
    glm::vec3 retVec = -d * rotMat;
    return retVec;
}

void NkCamera::rotatex(float amount)
{
    modelView = glm::rotate(modelView, amount, glm::vec3(1.0f, 0.0f, 0.0f));
}

void NkCamera::updateRot()
{
    modelView = glm::lookAt(position, position + direction, upRot);

}

void NkCamera::translateX(float amount)
{
    modelView= glm::translate(modelView,glm::vec3(amount, 0, 0));
}

void NkCamera::translateY(float amount)
{
    modelView= glm::translate(modelView,glm::vec3(0,amount, 0));
}

void NkCamera::translateZ(float amount)
{
    modelView= glm::translate(modelView,glm::vec3(0, 0, amount));
}

void NkCamera::setProjection3D(float fFOV, float fAspectRatio, float fNear, float fFar)
{
    projection = glm::perspective(fFOV, fAspectRatio, fNear, fFar);

}


