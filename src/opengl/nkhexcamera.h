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

private:
    glm::mat4 projection;  /**< Projection matrix */
    glm::mat4 modelView;   /**< View matrix */
    glm::vec3 position;    /**< position of camera */
};

#endif // NKHEXCAMERA_H
