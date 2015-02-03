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


#include "nkhex2.h"

NkHex2::NkHex2()
{

}

void NkHex2::init()
{
    FreeImage_Initialise(true);

    box = createHex();
    glBindVertexArray( box );
    positions.resize( 1 ); //make some space
    glGenBuffers( 1, &position_vbo ); //gen vbo
    glBindBuffer( GL_ARRAY_BUFFER, position_vbo ); //bind vbo
    //glBufferData( GL_ARRAY_BUFFER, sizeof( vec4 ) * positions.size(), &positions[0][0], GL_STATIC_DRAW );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vec4 ) * positions.size(), &positions[0][0], GL_STATIC_DRAW );
    GLuint location = 3;
    GLint components = 4;
    GLenum type = GL_FLOAT;
    GLboolean normalized = GL_FALSE;
    GLsizei datasize = sizeof( vec4 );
    char* pointer = 0; //no other components
    GLuint divisor = 1; //instanced

    glEnableVertexAttribArray( location ); //tell the location
    glVertexAttribPointer( location, components, type, normalized, datasize, pointer ); //tell other data
    glVertexAttribDivisor( location, divisor ); //is it instanced?

    program = 0;
    programsel = 0;
    this->loadShaders();
    //program1
    gbuffer_instanced_mvp_mat_loc = glGetUniformLocation( program, "mvp" );
    gbuffer_instanced_normal_mat_loc = glGetUniformLocation( program, "normal_mat" );
    gbuffer_instanced_view = glGetUniformLocation( program, "view" );
    //program2
    gbuffer_instanced_mvp_mat_loc_sel = glGetUniformLocation( programsel, "mvp" );
    gbuffer_instanced_normal_mat_loc_sel = glGetUniformLocation( programsel, "normal_mat" );
    gbuffer_instanced_view_sel = glGetUniformLocation( programsel, "view" );

    tex = frm.loadTexture("./data/textures/hextex.png");

}

void NkHex2::render(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView)
{
    glUseProgram(program);
    //instanced rendering
    glUniformMatrix4fv(gbuffer_instanced_mvp_mat_loc, 1, GL_FALSE, glm::value_ptr(*ProjectionMatrix));
    glUniformMatrix4fv(gbuffer_instanced_view, 1, GL_FALSE, glm::value_ptr(*mModelView));
    glBindVertexArray( box );

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    //upload the instance data
    glBindBuffer( GL_ARRAY_BUFFER, position_vbo ); //bind vbo

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
    glDrawElementsInstanced( GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0, HEX_SIZE);
    glBindSampler(0,0);
    glBindVertexArray( 0 );
    glUseProgram(0);

}
void NkHex2::renderSel(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView)
{
    glUseProgram(programsel);

    //instanced rendering
    glUniformMatrix4fv(gbuffer_instanced_mvp_mat_loc_sel, 1, GL_FALSE, glm::value_ptr(*ProjectionMatrix));
    glUniformMatrix4fv(gbuffer_instanced_view_sel, 1, GL_FALSE, glm::value_ptr(*mModelView));
    glBindVertexArray( box );

    //upload the instance data
    glBindBuffer( GL_ARRAY_BUFFER, position_vbo ); //bind vbo
    //you need to upload sizeof( vec4 ) * number_of_cubes bytes, DYNAMIC_DRAW because it is updated per frame

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_vbo);
    glDrawElementsInstanced( GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, 0, HEX_SIZE);
    glBindSampler(0,0);
    glBindVertexArray( 0 );
    glUseProgram(0);

}
void NkHex2::releaseScene()
{
    glDeleteProgram(program);
    glDeleteProgram(programsel);
}

GLuint NkHex2::createHex()
{
  vector<vec3> vertices;
  vector<vec3> normals;
  vector<vec2> texture;
  vector<unsigned int> indices;
  GLuint vao = 0;
  GLuint vertex_vbo = 0, normal_vbo = 0, index_vbo=0, tex_vbo=0;

  //up
  float hx = HEX_WIDTH/2;
  float ym = HEX_HEIGHT/3;
  float yh = HEX_HEIGHT-ym;
  vertices.push_back( vec3( 0, 0, 0 ) );
  vertices.push_back( vec3( -hx,0,  ym ) );
  vertices.push_back( vec3( 0, 0, yh ) );
  vertices.push_back( vec3( hx, 0, ym ) );
  vertices.push_back( vec3( hx, 0, -ym ) );
  vertices.push_back( vec3( 0, 0,-yh ) );
  vertices.push_back( vec3( -hx, 0, -ym) );

  indices.push_back( 0 );
  indices.push_back( 1 );
  indices.push_back( 2 );

  indices.push_back( 3 );
  indices.push_back( 4 );
  indices.push_back( 5 );
  indices.push_back( 6 );
  indices.push_back( 1 );

  normals.push_back( vec3( 0, 1, 0 ) );
  normals.push_back( vec3( 0, 1, 0 ) );
  normals.push_back( vec3( 0, 1, 0 ) );
  normals.push_back( vec3( 0, 1, 0 ) );
  normals.push_back( vec3( 0, 1, 0 ) );
  normals.push_back( vec3( 0, 1, 0 ) );
  normals.push_back( vec3( 0, 1, 0 ) );

  texture.push_back(vec2(0.5,0.5));
  texture.push_back(vec2(0,0.2));
  texture.push_back(vec2(0.5,0));
  texture.push_back(vec2(1,0.2));
  texture.push_back(vec2(1,0.8));
  texture.push_back(vec2(0.5,1));
  texture.push_back(vec2(0,0.8));

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 1, &vertex_vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vertex_vbo );
  glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vertices.size() * 3, &vertices[0][0],GL_STATIC_DRAW );
  glEnableVertexAttribArray( 0 );
  glVertexAttribPointer( 0, 3, GL_FLOAT, 0, 0, 0 );

  glGenBuffers( 1, &normal_vbo );
  glBindBuffer( GL_ARRAY_BUFFER, normal_vbo );
  glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * normals.size() * 3, &normals[0][0], GL_STATIC_DRAW );
  glEnableVertexAttribArray( 1 );
  glVertexAttribPointer( 1, 3, GL_FLOAT, 0, 0, 0 );

  glGenBuffers( 1, &tex_vbo );
  glBindBuffer( GL_ARRAY_BUFFER, tex_vbo );
  glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * texture.size() * 2, &texture[0][0], GL_STATIC_DRAW );
  glEnableVertexAttribArray( 2 );
  glVertexAttribPointer( 2, 2, GL_FLOAT, 0, 0, 0 );

  glGenBuffers( 1, &index_vbo );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * indices.size(), &indices[0], GL_STATIC_DRAW );
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray( 0 );

  return vao;
}

/**
* bind shaders with constants in compile time
* to avoid duplicate it through code.
*/
void NkHex2::loadShaders(){
    //shaders
    std::string n = std::to_string(HEX_LINE_NUM);
    std::string sidex = std::to_string(HEX_WIDTH);
    std::string sidey = std::to_string(HEX_HEIGHT);
    string vertexShaderSource = {
        "#version 330\n"
        "uniform mat4 mvp;\n"
        "uniform mat4 view;\n"
        "uniform mat3 normal_mat;\n"
        "layout(location=0) in vec4 in_vertex;\n"
        "layout(location=1) in vec3 in_normal;\n"
        "layout(location=2) in vec2 in_tex;\n"
        "flat out int InstanceID;\n"
        "out vec3 normal;\n"
        "out vec2 pass_TextureCoord;\n"
        "void main()\n"
        "{\n"
        "  vec4 vr= in_vertex;\n"
        "  int N="+n+";\n"
        "  int red=(gl_InstanceID/N);\n"
        "  vr.z += red*"+sidey+";\n"
        "  if (red%2!=0)\n"
        "    vr.x += "+sidex+"*(gl_InstanceID%N);\n"
        "  else\n"
        "    vr.x += "+sidex+"*(gl_InstanceID%N)+0.1;\n"
        "  normal = normal_mat * in_normal;\n"
        "  gl_Position = mvp * view * vr;\n"
        "  pass_TextureCoord = in_tex;\n"
        "  InstanceID = gl_InstanceID;\n"
        "}\n"
    };
    string fragmentShaderSource = {
        "#version 330\n"
        "uniform sampler2D gSampler;\n"
        "flat in int InstanceID;\n"
        "in vec3 normal;\n"
        "in vec2 pass_TextureCoord;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = texture2D(gSampler, pass_TextureCoord);\n"
        "}\n"
    };


    string selvertexShaderSource = {
        "#version 330\n"
        "uniform mat4 mvp;\n"
        "uniform mat4 view;\n"
        "uniform mat3 normal_mat;\n"
        "layout(location=0) in vec4 in_vertex;\n"
        "layout(location=1) in vec3 in_normal;\n"
        "layout(location=2) in vec2 in_tex;\n"
        "flat out int InstanceID;\n"
        "out vec4 pass_color;\n"
        "void main()\n"
        "{\n"
        "  vec4 vr= in_vertex;\n"
        "  int N="+n+";\n"
        "  int red=(gl_InstanceID/N);\n"
        "int col=(gl_InstanceID%N);\n"
        "  vr.z += red*"+sidey+";\n"
        "  if (red%2!=0)\n"
        "    vr.x += "+sidex+"*(gl_InstanceID%N);\n"
        "  else\n"
        "    vr.x += "+sidex+"*(gl_InstanceID%N)+0.1;\n"
        "  gl_Position = mvp * view * vr;\n"
        "if (red >= 128)\n"
        "    red=red+1;\n"
        "if (col >=128)\n"
        "    col=col+1;	\n"
        "float redf= red/256.0;\n"
        "float colf= col/256.0;\n"
        "pass_color =  vec4(redf,colf,0.5,1);\n"
        "  InstanceID = gl_InstanceID;\n"
        "}\n"
    };
    string selfragmentShaderSource = {
        "#version 330\n"
        "uniform sampler2D gSampler;\n"
        "flat in int InstanceID;\n"
        "in vec4 pass_color;\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "   color = pass_color;\n"
        "}\n"
    };

    frm.load_string_shader(vertexShaderSource, program, GL_VERTEX_SHADER);
    frm.load_string_shader(fragmentShaderSource, program, GL_FRAGMENT_SHADER);

    frm.load_string_shader(selvertexShaderSource, programsel, GL_VERTEX_SHADER);
    frm.load_string_shader(selfragmentShaderSource, programsel, GL_FRAGMENT_SHADER);
}
