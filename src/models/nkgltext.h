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

#ifndef NKGLTEXT_H
#define NKGLTEXT_H

#include <GL/glew.h>
#include <iostream>
#include <string>
#include "nkmodel.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

// Maximum texture width
#define MAXWIDTH 1024

struct point {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};

class NkGlText;
/**
 * @brief The atlas struct
 * create texture from font glyph
 */
struct atlas {
    GLuint tex;		// texture object

    int w;			// width of texture in pixels
    int h;			// height of texture in pixels

    struct {
        float ax;	// advance.x
        float ay;	// advance.y

        float bw;	// bitmap.width;
        float bh;	// bitmap.height;

        float bl;	// bitmap_left;
        float bt;	// bitmap_top;

        float tx;	// x offset of glyph in texture coordinates
        float ty;	// y offset of glyph in texture coordinates
    } c[128];		// character information

     atlas(FT_Face *face, int height,GLint &uniform_tex) {
        FT_Set_Pixel_Sizes(*face, 0, height);
        FT_GlyphSlot g = (*face)->glyph;

        int roww = 0;
        int rowh = 0;
         w = 0;
         h = 0;

         memset(c, 0, sizeof c);


        /* Find minimum size for a texture holding all visible ASCII characters */
        for (int i = 32; i < 128; i++) {
            if (FT_Load_Char(*face, i, FT_LOAD_RENDER)) {
                fprintf(stderr, "Loading character %c failed!\n", i);
                continue;
            }
            if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
                w = std::max(w, roww);
                h += rowh;
                roww = 0;
                rowh = 0;
            }
            roww += g->bitmap.width + 1;
            rowh = std::max(rowh, g->bitmap.rows);
        }

        w = std::max(w, roww);
        h += rowh;

        //glGetError();

        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(uniform_tex,0);

        unsigned char *pixels=new unsigned char[w*h];
        //da bi se ocistila pozadina
        memset(pixels, 0, sizeof(*pixels));
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,  GL_RED, GL_UNSIGNED_BYTE, pixels);

        delete[] pixels;
        /* Clamping to edges is important to prevent artifacts when scaling */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        /* Linear filtering usually looks best for text */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        /* Paste all glyph bitmaps into the texture, remembering the offset */
        int ox = 0;
        int oy = 0;

        rowh = 0;

        for (int i = 32; i < 128; i++) {
            if (FT_Load_Char(*face, i, FT_LOAD_RENDER)) {
                fprintf(stderr, "Loading character %c failed!\n", i);
                continue;
            }

            if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
                oy += rowh;
                rowh = 0;
                ox = 0;
            }

            glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);


            c[i].ax = g->advance.x >> 6;
            c[i].ay = g->advance.y >> 6;

            c[i].bw = g->bitmap.width;
            c[i].bh = g->bitmap.rows;

            c[i].bl = g->bitmap_left;
            c[i].bt = g->bitmap_top;

            c[i].tx = ox / (float)w;
            c[i].ty = oy / (float)h;

            rowh = std::max(rowh, g->bitmap.rows);
            ox += g->bitmap.width + 1;
        }

        //fprintf(stderr, "Generated a %d x %d (%d kb) texture atlas\n", w, h, w * h / 1024);
    }

    ~atlas() {
        glDeleteTextures(1, &tex);
    }

};
/**
 * @brief The NkGlText class
 * render text in OpenGL
 */
class NkGlText : public NkModel
{
public:

    NkGlText();
    ~NkGlText();
    int init(float w, float h);
    void render(glm::mat4 *ProjectionMatrix, glm::mat4 *mModelView);
    void renderClickMessage(std::string msg);
    void releaseScene();
    int init();
    GLuint createBuffer();
    void loadShaders();
private:
    void render_text(const char *text, atlas * a, float x, float y, float sx, float sy);

public:
    GLint uniform_tex;

    void renderText(std::string msg, int x, int y, glm::vec4 color, int size);
    int duzina() const;
    void setDuzina(int duzina);

    int visina() const;
    void setVisina(int visina);

private:
    GLuint program;
    GLint attribute_coord;

    GLint uniform_color;
    GLuint vbo;

    FT_Library *ft;
    FT_Face *face;
    const char *fontfilename;
    GLuint vao;

    atlas *a48;
    atlas *a24;
    atlas *a12;

    float m_duzina;
    float m_visina;


};

#endif // NKGLTEXT_H
