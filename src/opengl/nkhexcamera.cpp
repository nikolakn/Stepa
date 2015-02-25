#include "nkhexcamera.h"
#include <iostream>
NkHexCamera::NkHexCamera()
{
    position = glm::vec3(15,10,10);
    direction = glm::vec3(15, 0, 0);
    upRot = glm::vec3(0, 1, 0);
    setlookAt();
    zoom =0;
    rot=100;
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
    zoom+=amount;
    if(zoom>=10){
        zoom=10;
        return ;
    }
    if(zoom<=-16){
        zoom=-16;
        return;
    }

    //std::cout << zoom <<std::endl;
    modelView= glm::translate(modelView,glm::vec3(0,amount, 0));
}

void NkHexCamera::translateZ(float amount)
{
    modelView= glm::translate(modelView,glm::vec3(0, 0, amount));
}

void NkHexCamera::rotateDown(float amount)
{
    if(rot<=85) return;
    rot--;
    //std::cout << rot <<std::endl;
    float x=ExtractCameraPos_NoScale(modelView)[0];
    float y=ExtractCameraPos_NoScale(modelView)[1];
    float z=ExtractCameraPos_NoScale(modelView)[2];
    modelView= glm::translate(modelView,glm::vec3(x,y, z));
    modelView = glm::rotate(modelView, amount, glm::vec3(1.0f, 0.0f, 0.0f));
    modelView= glm::translate(modelView,glm::vec3(-x,-y, -z));
}

void NkHexCamera::rotateUp(float amount)
{
    //rotirati oko selektovanog ne oko kamere!!!!
    if(rot>=120) return;
    rot++;
    //std::cout << rot <<std::endl;
    float x=ExtractCameraPos_NoScale(modelView)[0];
    float y=ExtractCameraPos_NoScale(modelView)[1];
    float z=ExtractCameraPos_NoScale(modelView)[2];
    modelView= glm::translate(modelView,glm::vec3(x,y, z));
    modelView = glm::rotate(modelView, amount, glm::vec3(-1.0f, 0.0f, 0.0f));
    modelView= glm::translate(modelView,glm::vec3(-x,-y, -z));
}



