#include "nkselecthexmodel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
NkSelectHexModel::NkSelectHexModel()
{
}

NkSelectHexModel::~NkSelectHexModel()
{
    releaseScene();
}


int NkSelectHexModel::init()
{
    FreeImage_Initialise(true);
    setXY(74,49);
    selvao = createBuffer();
    glBindVertexArray( selvao );


    program = 0;
    this->loadShaders();
    //program1
    mvp_mat_loc = glGetUniformLocation( program, "mvp" );
    view_mat_loc = glGetUniformLocation( program, "view" );

    tex = frm.loadTexture("./data/textures/hexselect2.png");
    return 1;
}

void NkSelectHexModel::render(mat4 *ProjectionMatrix, mat4 *mModelView)
{
    glUseProgram(program);
    glEnable( GL_POLYGON_OFFSET_FILL );
    glPolygonOffset( -1, -1 );
    //instanced rendering
    glUniformMatrix4fv(mvp_mat_loc, 1, GL_FALSE, glm::value_ptr(*ProjectionMatrix));
    //glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, glm::value_ptr(*mModelView));
    glm::mat4 mCurrent;
    mCurrent = glm::translate(*mModelView, glm::vec3(x, 0, y));
    //mCurrent = glm::scale(mCurrent, glm::vec3(2.0, 2.0, 2.0));
    //mCurrent = glm::rotate(mCurrent, fRotationAngle*PIover180, glm::vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, glm::value_ptr(mCurrent));

    glBindVertexArray( selvao );

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBindBuffer( GL_ARRAY_BUFFER, index_vbo ); //bind vbo

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
    // Draw the vertices
    glDrawElements(GL_TRIANGLE_FAN, 6 ,GL_UNSIGNED_INT, 0);
    glDisable( GL_POLYGON_OFFSET_FILL );
    glBindSampler(0,0);
    glBindVertexArray( 0 );
    glUseProgram(0);
}

void NkSelectHexModel::releaseScene()
{
    glUseProgram(0);
    glDeleteTextures(1, &tex);
    glDeleteProgram(program);
    glDeleteBuffers(1,&vertex_vbo);
    glDeleteBuffers(1,&tex_vbo);
    glDeleteBuffers(1,&index_vbo);
    glDeleteVertexArrays(1, &selvao);
}


GLuint NkSelectHexModel::createBuffer()
{
    vector<vec3> vertices;
    vector<vec2> texture;
    vector<unsigned int> indices;
    GLuint vao = 0;
    vertex_vbo = 0;
    tex_vbo=0;
    index_vbo=0;
    vertices.push_back( vec3( -0.15f, 0, 0.16f ) );
    vertices.push_back( vec3( -0.15f,0, -0.16f ) );
    vertices.push_back( vec3( 0.15f, 0, -0.16f ) );
    vertices.push_back( vec3( 0.15f, 0, 0.16f ) );

    indices.push_back( 0 );
    indices.push_back( 3 );
    indices.push_back( 2 );
    indices.push_back( 2 );
    indices.push_back( 1 );
    indices.push_back( 0 );

    texture.push_back(vec2(0,0));
    texture.push_back(vec2(0,1));
    texture.push_back(vec2(1,1));
    texture.push_back(vec2(1,0));


    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glGenBuffers( 1, &vertex_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vertex_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vertices.size() * 3, &vertices[0][0],GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, 0, 0, 0 );

    glGenBuffers( 1, &tex_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, tex_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * texture.size() * 2, &texture[0][0], GL_STATIC_DRAW );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, 0, 0, 0 );

    glGenBuffers( 1, &index_vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * indices.size(), &indices[0], GL_STATIC_DRAW );
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    return vao;
}
void NkSelectHexModel::setXY(int xS, int yS) {
    // TODO Auto-generated method stub
    if (yS % 2 != 0 ){
        x = 0.2f*xS;
        y = 0.18f*yS;
    }
    else{
        x = 0.2f*xS+0.1f;
        y = 0.18f*yS;
    }
}
void NkSelectHexModel::loadShaders()
{
    //shaders
    //std::string n = std::to_string(HEX_LINE_NUM);
    //std::string sidex = std::to_string(HEX_WIDTH);
    //std::string sidey = std::to_string(HEX_HEIGHT);
    string vertexShaderSource = {
        "#version 330\n"
        "uniform mat4 mvp;\n"
        "uniform mat4 view;\n"
        "layout(location=0) in vec4 in_vertex;\n"
        "layout(location=1) in vec2 in_tex;\n"
        "out vec2 pass_TextureCoord;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = mvp * view * in_vertex;\n"
        "  pass_TextureCoord = in_tex;\n"
        "}\n"
    };
    string fragmentShaderSource = {
        "#version 330\n"
        "uniform sampler2D gSampler;\n"
        "in vec2 pass_TextureCoord;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = texture2D(gSampler, pass_TextureCoord);\n"
        "}\n"
    };
    frm.load_string_shader(vertexShaderSource, program, GL_VERTEX_SHADER);
    frm.load_string_shader(fragmentShaderSource, program, GL_FRAGMENT_SHADER);
}


int NkSelectHexModel::init(float, float)
{
    FreeImage_Initialise(true);
    setXY(74,49);
    selvao = createBuffer();
    glBindVertexArray( selvao );


    program = 0;
    this->loadShaders();
    //program1
    mvp_mat_loc = glGetUniformLocation( program, "mvp" );
    view_mat_loc = glGetUniformLocation( program, "view" );

    tex = frm.loadTexture("./data/textures/hexselect2.png");
    return 1;
}
