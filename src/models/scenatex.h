#include "models/nkmodel.h"

class NkScenaTex : public NkModel
{
private:

    CVertexBufferObject *vboSceneObjects;
    unsigned int uiVAO; // And one VAO
    CTexture tGold, tSnow;

float fRotationAngle;
const float PIover180 = 3.1415f/180.0f;
float fRotationAngleCube , fRotationAnglePyramid;
float fCubeRotationSpeed , fPyramidRotationSpeed;

public:
    NkScenaTex();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void init();
    void render(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView);
    void releaseScene();
	
};
