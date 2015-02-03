
#include "models/scenatex.h"

#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()

NkScenaTex::NkScenaTex()
{
    //init();
}

void NkScenaTex::moveLeft()
{
    //fPyramidRotationSpeed -= 2.0f;
    fRotationAnglePyramid += 4.0f;
    fRotationAnglePyramid += fPyramidRotationSpeed;
}

void NkScenaTex::moveRight()
{
    //fPyramidRotationSpeed += 2.0f;
    fRotationAnglePyramid -= 4.0f;
}

void NkScenaTex::moveUp()
{
    //fCubeRotationSpeed += 2.0f;
    fRotationAngleCube += 4.0f;
}

void NkScenaTex::moveDown()
{
    //fCubeRotationSpeed -= 2.0f;
    fRotationAngleCube -= 4.0f;
}

void NkScenaTex::init()
{
    fRotationAngle = 0.0;
    fRotationAngleCube = 0.0f;
    fRotationAnglePyramid = 0.0f;
    fCubeRotationSpeed = 0.0f;
    fPyramidRotationSpeed = 0.0f;
    vboSceneObjects=new CVertexBufferObject();
    glm::vec3 vCubeVertices[] =
    {
        // Front face
        glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
        // Back face
        glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),

        // Left face
        glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
        // Right face
        glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
        // Top face
        glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
        // Bottom face
        glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f)
    };

    glm::vec2 vCubeTexCoords[] = {glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f),
                                  glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)};

    glm::vec3 vPyramidVertices[] =
    {
        // Front face
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
        // Back face
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
        // Left face
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
        // Right face
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f)
    };

    glm::vec2 vPyramidTexCoords[] = {glm::vec2(0.5f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f)};

    glm::vec3 vGround[] =
    {
        glm::vec3(-50.0, -10.0, -50.0), glm::vec3(50.0, -10.0, -50.0), glm::vec3(50.0, -10.0, 50.0), glm::vec3(50.0, -10.0, 50.0), glm::vec3(-50.0, -10.0, 50.0), glm::vec3(-50.0, -10.0, -50.0)
    };

    vboSceneObjects->createVBO();
    glGenVertexArrays(1, &uiVAO); // Create one VAO
    glBindVertexArray(uiVAO);

    vboSceneObjects->bindVBO();

    // Add cube to VBO
    FOR(i, 36)
    {
        vboSceneObjects->addData(&vCubeVertices[i], sizeof(glm::vec3));
        vboSceneObjects->addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
    }

    // Add pyramid to VBO

    FOR(i, 12)
    {
        vboSceneObjects->addData(&vPyramidVertices[i], sizeof(glm::vec3));
        vboSceneObjects->addData(&vPyramidTexCoords[i%3], sizeof(glm::vec2));
    }

    // Add ground to VBO

    FOR(i, 6)
    {
        vboSceneObjects->addData(&vGround[i], sizeof(glm::vec3));
        vCubeTexCoords[i] *= 5.0f;
        vboSceneObjects->addData(&vCubeTexCoords[i%6], sizeof(glm::vec2));
    }

    vboSceneObjects->uploadDataToGPU(GL_STATIC_DRAW);

    // Vertex positions start on zero index, and distance between two consecutive is sizeof whole
    // vertex data (position and tex. coord)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), 0);
    // Texture coordinates start right after positon, thus on (sizeof(glm::vec3)) index,
    // and distance between two consecutive is sizeof whole vertex data (position and tex. coord)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));

    // Load shaders and create shader program
    program = LoadShaders("./data/shaders/shadertex.vert", "./data/shaders/shadertex.frag");
    if(program == 0){
        std::cout<<"grska shader NKscenaTex" << std::endl;
    }

    tGold.loadTexture2D("./data/textures/golddiag.jpg", true);
    tGold.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

    tSnow.loadTexture2D("./data/textures/snow.jpg", true);
    tSnow.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);
    glBindVertexArray(0);

}


void NkScenaTex::render(glm::mat4 *ProjectionMatrix,glm::mat4 *mModelView){

     glUseProgram(program);

    int iModelViewLoc = glGetUniformLocation(program, "modelViewMatrix");
    int iProjectionLoc = glGetUniformLocation(program, "projectionMatrix");
    glUniformMatrix4fv(iProjectionLoc, 1, GL_FALSE, glm::value_ptr(*ProjectionMatrix));

    glm::mat4 mCurrent;

    glBindVertexArray(uiVAO);
    tGold.bindTexture(0);

    // Rendering of pyramid

    mCurrent = glm::translate(*mModelView, glm::vec3(8.0f, 0.0f, 0.0f));
    mCurrent = glm::scale(mCurrent, glm::vec3(10.0f, 10.0f, 10.0f));
    mCurrent = glm::rotate(mCurrent, fRotationAnglePyramid*PIover180, glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));

    glDrawArrays(GL_TRIANGLES, 36, 12);

    // Rendering of cube

    mCurrent = glm::translate(*mModelView, glm::vec3(-8.0f, 0.0f, 0.0f));
    mCurrent = glm::scale(mCurrent, glm::vec3(10.0f, 10.0f, 10.0f));
    mCurrent = glm::rotate(mCurrent, fRotationAngleCube*PIover180, glm::vec3(1.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(mCurrent));

    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Render ground

    tSnow.bindTexture(0);

    glUniformMatrix4fv(iModelViewLoc, 1, GL_FALSE, glm::value_ptr(*mModelView));
    glDrawArrays(GL_TRIANGLES, 48, 6);

    glBindSampler(0,0);
    glBindVertexArray(0);
    glUseProgram(0);
}


void NkScenaTex::releaseScene()
{
    glDeleteShader(program);



    vboSceneObjects->releaseVBO();
    delete  vboSceneObjects;
    glDeleteVertexArrays(1, &uiVAO);

    tGold.releaseTexture();
    tSnow.releaseTexture();
}
