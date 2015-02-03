/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; version 2
* of the License.
*/


#ifndef framework_h
#define framework_h

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <streambuf>
#include <list>
#include <vector>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <FreeImage.h>

using namespace glm;
using namespace std;


/**
 * @brief The framework class
 * various utility functiions for loading textures
 * load, compile and link shaders
 */
class framework
{
public:
    static const unsigned int INFOLOG_SIZE = 4097;

public:
    // Method to load an image into a texture using the freeimageplus library. Returns the texture ID or dies trying.
    GLuint loadTexture(string filenameString,GLenum minificationFilter = GL_LINEAR, GLenum magnificationFilter = GL_LINEAR)
    {
        //glGetError();
        // Get the filename as a pointer to a const char array to play nice with FreeImage
        const char* filename = filenameString.c_str();

        // Determine the format of the image.
        // Note: The second paramter ('size') is currently unused, and we should use 0 for it.
        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename , 0);

        // Image not found? Abort! Without this section we get a 0 by 0 image with 0 bits-per-pixel but we don't abort, which
        // you might find preferable to dumping the user back to the desktop.
        if (format == -1)
        {
            cout << "Could not find image: " << filenameString << " - Aborting." << endl;
            exit(-1);
        }

        // Found image, but couldn't determine the file format? Try again...
        if (format == FIF_UNKNOWN)
        {
            cout << "Couldn't determine file format - attempting to get from file extension..." << endl;

            // ...by getting the filetype from the filename extension (i.e. .PNG, .GIF etc.)
            // Note: This is slower and more error-prone that getting it from the file itself,
            // also, we can't use the 'U' (unicode) variant of this method as that's Windows only.
            format = FreeImage_GetFIFFromFilename(filename);

            // Check that the plugin has reading capabilities for this format (if it's FIF_UNKNOWN,
            // for example, then it won't have) - if we can't read the file, then we bail out =(
            if ( !FreeImage_FIFSupportsReading(format) )
            {
                cout << "Detected image format cannot be read!" << endl;
                exit(-1);
            }
        }

        // If we're here we have a known image format, so load the image into a bitap
        FIBITMAP* bitmap = FreeImage_Load(format, filename);

        // How many bits-per-pixel is the source image?
        int bitsPerPixel =  FreeImage_GetBPP(bitmap);

        // Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
        // but only if the image is not already 32 bits (i.e. 8 bits per channel).
        // Note: ConvertTo32Bits returns a CLONE of the image data - so if we
        // allocate this back to itself without using our bitmap32 intermediate
        // we will LEAK the original bitmap data, and valgrind will show things like this:
        //
        // LEAK SUMMARY:
        //  definitely lost: 24 bytes in 2 blocks
        //  indirectly lost: 1,024,874 bytes in 14 blocks    <--- Ouch.
        //
        // Using our intermediate and cleaning up the initial bitmap data we get:
        //
        // LEAK SUMMARY:
        //  definitely lost: 16 bytes in 1 blocks
        //  indirectly lost: 176 bytes in 4 blocks
        //
        // All above leaks (192 bytes) are caused by XGetDefault (in /usr/lib/libX11.so.6.3.0) - we have no control over this.
        //
        FIBITMAP* bitmap32;
        if (bitsPerPixel == 32)
        {
            //cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << endl;
            bitmap32 = bitmap;
        }
        else
        {
            //cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << endl;
            bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
        }

        // Some basic image info - strip it out if you don't care
        int imageWidth  = FreeImage_GetWidth(bitmap32);
        int imageHeight = FreeImage_GetHeight(bitmap32);
        //cout << "Image: " << filenameString << " is size: " << imageWidth << "x" << imageHeight << "." << endl;

        // Get a pointer to the texture data as an array of unsigned bytes.
        // Note: At this point bitmap32 ALWAYS holds a 32-bit colour version of our image - so we get our data from that.
        // Also, we don't need to delete or delete[] this textureData because it's not on the heap (so attempting to do
        // so will cause a crash) - just let it go out of scope and the memory will be returned to the stack.
        GLubyte* textureData = FreeImage_GetBits(bitmap32);

        // Generate a texture ID and bind to it
        GLuint tempTextureID;
        glGenTextures(1, &tempTextureID);
        glBindTexture(GL_TEXTURE_2D, tempTextureID);

        // Construct the texture.
        // Note: The 'Data format' is the format of the image data as provided by the image library. FreeImage decodes images into
        // BGR/BGRA format, but we want to work with it in the more common RGBA format, so we specify the 'Internal format' as such.
        glTexImage2D(GL_TEXTURE_2D,    // Type of texture
                     0,                // Mipmap level (0 being the top level i.e. full size)
                     GL_RGBA,          // Internal format
                     imageWidth,       // Width of the texture
                     imageHeight,      // Height of the texture,
                     0,                // Border in pixels
                     GL_BGRA,          // Data format
                     GL_UNSIGNED_BYTE, // Type of texture data
                     textureData);     // The image data to use for this texture

        // Specify our minification and magnification filters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);

        // If we're using MipMaps, then we'll generate them here.
        // Note: The glGenerateMipmap call requires OpenGL 3.0 as a minimum.
        if (minificationFilter == GL_LINEAR_MIPMAP_LINEAR   ||
                minificationFilter == GL_LINEAR_MIPMAP_NEAREST  ||
                minificationFilter == GL_NEAREST_MIPMAP_LINEAR  ||
                minificationFilter == GL_NEAREST_MIPMAP_NEAREST)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        /*
        // Check for OpenGL texture creation errors
        GLenum glError = glGetError();
        if(glError)
        {
            cout << "There was an error loading the texture: "<< filenameString << endl;

            switch (glError)
            {
                case GL_INVALID_ENUM:
                    cout << "Invalid enum." << endl;
                    break;

                case GL_INVALID_VALUE:
                    cout << "Invalid value." << endl;
                    break;

                case GL_INVALID_OPERATION:
                    cout << "Invalid operation." << endl;

                default:
                    cout << "Unrecognised GLenum." << endl;
                    break;
            }

            //cout << "See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details." << endl;
        }
        */
        // Unload the 32-bit colour bitmap
        FreeImage_Unload(bitmap32);

        // If we had to do a conversion to 32-bit colour, then unload the original
        // non-32-bit-colour version of the image data too. Otherwise, bitmap32 and
        // bitmap point at the same data, and that data's already been free'd, so
        // don't attempt to free it again! (or we'll crash).
        if (bitsPerPixel != 32)
        {
            FreeImage_Unload(bitmap);
        }

        // Finally, return the texture ID
        return tempTextureID;
    }


    void load_string_shader(string text, GLuint& program, const GLenum& type)
    {
        if( !program ) program = glCreateProgram();

        //std::cout << text<< std::endl  ;
        compile_shader( text.c_str(), program, type );
        link_shader( program );
    }

    void compile_shader( const char* text, const GLuint& program, const GLenum& type ) const
    {
        GLchar infolog[INFOLOG_SIZE];

        GLuint id = glCreateShader( type );
        glShaderSource( id, 1, &text, 0 );
        glCompileShader( id );

        GLint success;
        glGetShaderiv( id, GL_COMPILE_STATUS, &success );

        if( !success )
        {
            glGetShaderInfoLog( id, INFOLOG_SIZE, 0, infolog );
            cout << infolog << endl;
        }
        else
        {
            glAttachShader( program, id );
            glDeleteShader( id );
        }
    }

    void link_shader( const GLuint& shader_program ) const
    {
        glLinkProgram( shader_program );

        GLint success;
        glGetProgramiv( shader_program, GL_LINK_STATUS, &success );

        if( !success )
        {
            GLchar infolog[INFOLOG_SIZE];
            glGetProgramInfoLog( shader_program, INFOLOG_SIZE, 0, infolog );
            cout << infolog << endl;
        }

        glValidateProgram( shader_program );

        glGetProgramiv( shader_program, GL_VALIDATE_STATUS, &success );

        if( !success )
        {
            GLchar infolog[INFOLOG_SIZE];
            glGetProgramInfoLog( shader_program, INFOLOG_SIZE, 0, infolog );
            cout << infolog << endl;
        }
    }

    void load_shader( GLuint& program, const GLenum& type, const string& filename ) const
    {
        ifstream f( filename );

        if( !f ) cout << "Couldn't load shader: " << filename << endl;

        string str( ( istreambuf_iterator<char>( f ) ),
                    istreambuf_iterator<char>() );

        if( !program ) program = glCreateProgram();

        compile_shader( str.c_str(), program, type );
        link_shader( program );
    }

    GLuint create_quad( const vec3& ll, const vec3& lr, const vec3& ul, const vec3& ur ) const
    {
        vector<vec3> vertices;
        vector<vec2> tex_coords;
        vector<unsigned int> indices;
        GLuint vao = 0;
        GLuint vertex_vbo = 0, tex_coord_vbo = 0, index_vbo = 0;

        indices.push_back( 0 );
        indices.push_back( 1 );
        indices.push_back( 2 );

        indices.push_back( 0 );
        indices.push_back( 2 );
        indices.push_back( 3 );

        /*vertices.push_back( vec3( -1, -1, 0 ) );
        vertices.push_back( vec3( 1, -1, 0 ) );
        vertices.push_back( vec3( 1, 1, 0 ) );
        vertices.push_back( vec3( -1, 1, 0 ) );*/
        vertices.push_back( ll );
        vertices.push_back( lr );
        vertices.push_back( ur );
        vertices.push_back( ul );

        tex_coords.push_back( vec2( 0, 0 ) );
        tex_coords.push_back( vec2( 1, 0 ) );
        tex_coords.push_back( vec2( 1, 1 ) );
        tex_coords.push_back( vec2( 0, 1 ) );

        glGenVertexArrays( 1, &vao );
        glBindVertexArray( vao );

        glGenBuffers( 1, &vertex_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, vertex_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vertices.size() * 3, &vertices[0][0], GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, 0, 0, 0 );

        glGenBuffers( 1, &tex_coord_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, tex_coord_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * tex_coords.size() * 2, &tex_coords[0][0], GL_STATIC_DRAW );
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, 0, 0, 0 );

        glGenBuffers( 1, &index_vbo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * indices.size(), &indices[0], GL_STATIC_DRAW );

        glBindVertexArray( 0 );

        return vao;
    }

    inline GLuint create_box() const
    {
        vector<vec3> vertices;
        vector<vec3> normals;
        vector<unsigned int> indices;
        GLuint vao = 0;
        GLuint vertex_vbo = 0, normal_vbo = 0, index_vbo = 0;

        //front
        vertices.push_back( vec3( -1, -1, 1 ) );
        vertices.push_back( vec3( 1, -1, 1 ) );
        vertices.push_back( vec3( 1, 1, 1 ) );
        vertices.push_back( vec3( -1, 1, 1 ) );

        indices.push_back( 0 );
        indices.push_back( 1 );
        indices.push_back( 2 );

        indices.push_back( 0 );
        indices.push_back( 2 );
        indices.push_back( 3 );

        normals.push_back( vec3( 0, 0, 1 ) );
        normals.push_back( vec3( 0, 0, 1 ) );
        normals.push_back( vec3( 0, 0, 1 ) );
        normals.push_back( vec3( 0, 0, 1 ) );

        //back
        vertices.push_back( vec3( -1, -1, -1 ) );
        vertices.push_back( vec3( 1, -1, -1 ) );
        vertices.push_back( vec3( 1, 1, -1 ) );
        vertices.push_back( vec3( -1, 1, -1 ) );

        indices.push_back( 6 );
        indices.push_back( 5 );
        indices.push_back( 4 );

        indices.push_back( 7 );
        indices.push_back( 6 );
        indices.push_back( 4 );

        normals.push_back( vec3( 0, 0, -1 ) );
        normals.push_back( vec3( 0, 0, -1 ) );
        normals.push_back( vec3( 0, 0, -1 ) );
        normals.push_back( vec3( 0, 0, -1 ) );

        //left
        vertices.push_back( vec3( -1, -1, 1 ) );
        vertices.push_back( vec3( -1, -1, -1 ) );
        vertices.push_back( vec3( -1, 1, 1 ) );
        vertices.push_back( vec3( -1, 1, -1 ) );

        indices.push_back( 10 );
        indices.push_back( 9 );
        indices.push_back( 8 );

        indices.push_back( 11 );
        indices.push_back( 9 );
        indices.push_back( 10 );

        normals.push_back( vec3( -1, 0, 0 ) );
        normals.push_back( vec3( -1, 0, 0 ) );
        normals.push_back( vec3( -1, 0, 0 ) );
        normals.push_back( vec3( -1, 0, 0 ) );

        //right
        vertices.push_back( vec3( 1, -1, 1 ) );
        vertices.push_back( vec3( 1, -1, -1 ) );
        vertices.push_back( vec3( 1, 1, 1 ) );
        vertices.push_back( vec3( 1, 1, -1 ) );

        indices.push_back( 12 );
        indices.push_back( 13 );
        indices.push_back( 14 );

        indices.push_back( 14 );
        indices.push_back( 13 );
        indices.push_back( 15 );

        normals.push_back( vec3( 1, 0, 0 ) );
        normals.push_back( vec3( 1, 0, 0 ) );
        normals.push_back( vec3( 1, 0, 0 ) );
        normals.push_back( vec3( 1, 0, 0 ) );

        //up
        vertices.push_back( vec3( -1, 1, 1 ) );
        vertices.push_back( vec3( 1, 1, 1 ) );
        vertices.push_back( vec3( 1, 1, -1 ) );
        vertices.push_back( vec3( -1, 1, -1 ) );

        indices.push_back( 16 );
        indices.push_back( 17 );
        indices.push_back( 18 );

        indices.push_back( 16 );
        indices.push_back( 18 );
        indices.push_back( 19 );

        normals.push_back( vec3( 0, 1, 0 ) );
        normals.push_back( vec3( 0, 1, 0 ) );
        normals.push_back( vec3( 0, 1, 0 ) );
        normals.push_back( vec3( 0, 1, 0 ) );

        //down
        vertices.push_back( vec3( -1, -1, 1 ) );
        vertices.push_back( vec3( 1, -1, 1 ) );
        vertices.push_back( vec3( 1, -1, -1 ) );
        vertices.push_back( vec3( -1, -1, -1 ) );

        indices.push_back( 22 );
        indices.push_back( 21 );
        indices.push_back( 20 );

        indices.push_back( 23 );
        indices.push_back( 22 );
        indices.push_back( 20 );

        normals.push_back( vec3( 0, -1, 0 ) );
        normals.push_back( vec3( 0, -1, 0 ) );
        normals.push_back( vec3( 0, -1, 0 ) );
        normals.push_back( vec3( 0, -1, 0 ) );

        glGenVertexArrays( 1, &vao );
        glBindVertexArray( vao );

        glGenBuffers( 1, &vertex_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, vertex_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * vertices.size() * 3, &vertices[0][0], GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, 0, 0, 0 );

        glGenBuffers( 1, &normal_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, normal_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * normals.size() * 3, &normals[0][0], GL_STATIC_DRAW );
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 3, GL_FLOAT, 0, 0, 0 );

        glGenBuffers( 1, &index_vbo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_vbo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * indices.size(), &indices[0], GL_STATIC_DRAW );

        glBindVertexArray( 0 );

        return vao;
    }

    void get_opengl_error( const bool& ignore = false ) const
    {
        bool got_error = false;
        GLenum error = 0;
        error = glGetError();
        string errorstring = "";

        while( error != GL_NO_ERROR )
        {
            if( error == GL_INVALID_ENUM )
            {
                //An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.
                errorstring += "OpenGL error: invalid enum...\n";
                got_error = true;
            }

            if( error == GL_INVALID_VALUE )
            {
                //A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.
                errorstring += "OpenGL error: invalid value...\n";
                got_error = true;
            }

            if( error == GL_INVALID_OPERATION )
            {
                //The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.
                errorstring += "OpenGL error: invalid operation...\n";
                got_error = true;
            }

            if( error == GL_STACK_OVERFLOW )
            {
                //This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.
                errorstring += "OpenGL error: stack overflow...\n";
                got_error = true;
            }

            if( error == GL_STACK_UNDERFLOW )
            {
                //This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.
                errorstring += "OpenGL error: stack underflow...\n";
                got_error = true;
            }

            if( error == GL_OUT_OF_MEMORY )
            {
                //There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.
                errorstring += "OpenGL error: out of memory...\n";
                got_error = true;
            }

            if( error == GL_TABLE_TOO_LARGE )
            {
                //The specified table exceeds the implementation's maximum supported table size.  The offending command is ignored and has no other side effect than to set the error flag.
                errorstring += "OpenGL error: table too large...\n";
                got_error = true;
            }

            error = glGetError();
        }

        if( got_error && !ignore )
        {
            cerr << errorstring;
            return;
        }
    }

    void check_fbo_status( const GLenum& target = GL_FRAMEBUFFER )
    {
        GLenum error_code = glCheckFramebufferStatus( target );

        switch( error_code )
        {
        case GL_FRAMEBUFFER_COMPLETE:
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT." << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS." << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT." << endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            cerr << "GL_FRAMEBUFFER_UNSUPPORTED." << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER." << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER." << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE." << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS." << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT." << endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            cerr << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS." << endl;
            break;
        default:
            cerr << "Unknown Frame Buffer error cause: " << error_code << endl;
            break;
        }
    }

    GLuint LoadShadersFromFile(const char * vertex_file_path,const char * fragment_file_path){

        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        // Read the Vertex Shader code from the file
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
        if(VertexShaderStream.is_open()){
            std::string Line = "";
            while(getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
        }else{
            printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
            getchar();
            return 0;
        }

        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if(FragmentShaderStream.is_open()){
            std::string Line = "";
            while(getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
        }

        GLint Result = GL_FALSE;
        int InfoLogLength;

        // Compile Vertex Shader
        //printf("Compiling shader : %s\n", vertex_file_path);
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader(VertexShaderID);

        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(Result == GL_FALSE){
            if ( InfoLogLength > 0 ){
                std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
                printf("%s\n", &VertexShaderErrorMessage[0]);
            }
        }

        // Compile Fragment Shader
        //printf("Compiling shader : %s\n", fragment_file_path);
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader(FragmentShaderID);

        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(Result == GL_FALSE){
            if ( InfoLogLength > 0 ){
                std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
                glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
                printf("%s\n", &FragmentShaderErrorMessage[0]);
            }
        }

        // Link the program
        //printf("Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);

        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
        if(Result == GL_FALSE){
            if ( InfoLogLength > 0 ){
                std::vector<char> ProgramErrorMessage(InfoLogLength+1);
                glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
                printf("%s\n", &ProgramErrorMessage[0]);
            }
        }
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);

        return ProgramID;
    }

};

#endif
