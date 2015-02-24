#include "nkhexcamera.h"

NkHexCamera::NkHexCamera()
{
    position = glm::vec3(15,10,10);
    direction = glm::vec3(15, 0, 0);
    upRot = glm::vec3(0, 1, 0);
    setlookAt();
}

void NkHexCamera::setlookAt()
{
    modelView= glm::lookAt(position, direction,  upRot);
}

void NkHexCamera::setModelView(const glm::mat4 &mv)
{
    modelView = mv;
}

void NkHexCamera::init()
{
    position = glm::vec3(15,10,10);
    direction = glm::vec3(15, 0, 0);
    upRot = glm::vec3(0, 1, 0);
    setlookAt();
}


glm::vec3 NkHexCamera::ExtractCameraPos_NoScale(const glm::mat4 & a_modelView)
{
    glm::mat3 rotMat(a_modelView);
    glm::vec3 d(a_modelView[3]);
    glm::vec3 retVec = -d * rotMat;
    return retVec;
}

void NkHexCamera::setProjection3D(float fFOV, float fAspectRatio, float fNear, float fFar)
{
    projection = glm::perspective(fFOV, fAspectRatio, fNear, fFar);

}

void NkHexCamera::rotate(float amount)
{
    modelView = glm::rotate(modelView, amount, glm::vec3(0.0f, 1.0f, 0.0f));
}

void NkHexCamera::rotatex(float amount)
{
    modelView = glm::rotate(modelView, amount, glm::vec3(1.0f, 0.0f, 0.0f));
}

void NkHexCamera::updateRot()
{
    modelView = glm::lookAt(position, position + direction, upRot);

}

void NkHexCamera::translateX(float amount)
{
    modelView= glm::translate(modelView,glm::vec3(amount, 0, 0));
}

void NkHexCamera::translateY(float amount)
{
    modelView= glm::translate(modelView,glm::vec3(0,amount, 0));
}

void NkHexCamera::translateZ(float amount)
{
    modelView= glm::translate(modelView,glm::vec3(0, 0, amount));
}

void NkHexCamera::rotateDown(float amount)
{
    float x=ExtractCameraPos_NoScale(modelView)[0];
    float y=ExtractCameraPos_NoScale(modelView)[1];
    float z=ExtractCameraPos_NoScale(modelView)[2];
    modelView= glm::translate(modelView,glm::vec3(x,y, z));
    modelView = glm::rotate(modelView, amount, glm::vec3(1.0f, 0.0f, 0.0f));
    modelView= glm::translate(modelView,glm::vec3(-x,-y, -z));
}

void NkHexCamera::rotateUp(float amount)
{
    float x=ExtractCameraPos_NoScale(modelView)[0];
    float y=ExtractCameraPos_NoScale(modelView)[1];
    float z=ExtractCameraPos_NoScale(modelView)[2];
    modelView= glm::translate(modelView,glm::vec3(x,y, z));
    modelView = glm::rotate(modelView, amount, glm::vec3(-1.0f, 0.0f, 0.0f));
    modelView= glm::translate(modelView,glm::vec3(-x,-y, -z));
}



