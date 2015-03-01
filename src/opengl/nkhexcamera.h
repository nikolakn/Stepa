#ifndef NKHEXCAMERA_H
#define NKHEXCAMERA_H

#define GLM_FORCE_RADIANS
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class NkHexCamera
{
public:
    NkHexCamera();

    glm::mat4* getProjectionMat(void) {return &projection;}
    glm::mat4* getModelView() {return &modelView;}

    void setlookAt();
    void setPogled();
    void rotateDown();
    void rotateUp();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void zoomIn();
    void zoomOut();


private:
    void setProjection3D(float fFOV, float fAspectRatio, float fNear, float fFar);

    void setModelView(const glm::mat4 &mv);
    void init();

    void translateX(float amount);
    glm::vec3 ExtractCameraPos_NoScale(const glm::mat4 &a_modelView);

    void updateRot();
    void rotate(float amount);
    void rotatex(float amount);
    void translateY(float amount);
    void translateZ(float amount);

private:
    glm::mat4 projection;  /**< Projection matrix */
    glm::mat4 modelView;   /**< View matrix */

    glm::vec3 position;    /**< position of camera */
    glm::vec3 direction;   /**< look at point */
    glm::vec3 upRot;       /**< Rotation of camera */

    float zoom;
    float rot;
    const float pomak=0.5;
    const float rotpomak=0.05;
};

#endif // NKHEXCAMERA_H
