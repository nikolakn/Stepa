#include "nkloadscreen.h"

NkLoadscreen::NkLoadscreen()
{

}

NkLoadscreen::~NkLoadscreen()
{
    releaseScene();
}



void NkLoadscreen::render(mat4 *, mat4 *)
{
    glUseProgram(program);
    glEnable( GL_POLYGON_OFFSET_FILL );
    glPolygonOffset( 1, 1 );

    glBindVertexArray( selvao );

    glBindBuffer( GL_ARRAY_BUFFER, index_vbo ); //bind vbo

    glDrawElements(GL_TRIANGLE_FAN, 6 ,GL_UNSIGNED_INT, 0);
    glDisable( GL_POLYGON_OFFSET_FILL );
    glBindSampler(0,0);
    glBindVertexArray( 0 );
    glUseProgram(0);
}

void NkLoadscreen::releaseScene()
{
    glDeleteProgram(program);
    glDeleteBuffers(1,&vertex_vbo);
    glDeleteBuffers(1,&index_vbo);
    glDeleteVertexArrays(1, &selvao);
}

GLuint NkLoadscreen::createBuffer()
{
    vector<vec3> vertices;
    vector<unsigned int> indices;
    GLuint vao = 0;
    vertex_vbo = 0;
    index_vbo=0;
    vertices.push_back( vec3( -1.0f, 1.0f,0.0f ) );
    vertices.push_back( vec3( -1.0f,-1.0f, 0 ) );
    vertices.push_back( vec3( 1.0f, -1.0f, 0 ) );
    vertices.push_back( vec3( 1.0f, 1.0f, 0 ) );

    indices.push_back( 0 );
    indices.push_back( 3 );
    indices.push_back( 2 );
    indices.push_back( 2 );
    indices.push_back( 1 );
    indices.push_back( 0 );

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glGenBuffers( 1, &vertex_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vertex_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vertices.size() * 3, &vertices[0][0],GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, 0, 0, 0 );


    glGenBuffers( 1, &index_vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * indices.size(), &indices[0], GL_STATIC_DRAW );

    glBindVertexArray( 0 );
    return vao;
}

void NkLoadscreen::loadShaders()
{
    string vertexShaderSource = {
        "#version 330\n"
        "uniform mat4 view;\n"
        "layout(location=0) in vec4 in_vertex;\n"
        "void main()\n"
        "{\n"
        "  gl_Position =in_vertex;\n"
        "}\n"
    };
    string fragmentShaderSource = {
        "#version 330\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.0,0.0,0.0,1);\n"
        "}\n"
    };
    frm.load_string_shader(vertexShaderSource, program, GL_VERTEX_SHADER);
    frm.load_string_shader(fragmentShaderSource, program, GL_FRAGMENT_SHADER);
}

int NkLoadscreen::init()
{
    FreeImage_Initialise(true);
    selvao = createBuffer();
    glBindVertexArray( selvao );
    program = 0;
    this->loadShaders();
    view_mat_loc = glGetUniformLocation( program, "view" );
    return 1;
}

int NkLoadscreen::init(float w, float h)
{
    m_duzina=w;
    m_visina=h;
    FreeImage_Initialise(true);
    selvao = createBuffer();
    glBindVertexArray( selvao );

    program = 0;
    this->loadShaders();
    view_mat_loc = glGetUniformLocation( program, "view" );
    return 1;
}
