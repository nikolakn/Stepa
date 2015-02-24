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

/** @file opengl.h
 *  @brief Initialize OpenGL and render scene
 *
 *  @author Nikola Knezevic (nkcodeplus@gmail.com)
 *  @bug No known bugs.
 */


#ifndef OPENGL_H
#define OPENGL_H

#include <GL/glew.h>

#ifdef WIN32
#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif

#include <vector>

#include "opengl/camera.h"
#include "opengl/nkhexcamera.h"
#include "models/nkmodel.h"
#include "models/nkhex2.h"
#include "models/nkgltext.h"
#include "models/nkselecthexmodel.h"
#include "models/nkbleckrect.h"
#include "models/nkmapa.h"
#include "models/nkloadscreen.h"
#include "models/nkunitrender.h"
using namespace std;

/**
 * @brief The NkOpengl class
 * Initialize OpenGL , load models data
 * render scene
 * control camera
 */
class NkOpengl
{
public:
    NkOpengl();
    ~NkOpengl();

    /**
     * @brief initGL Initialize OpenGL
     * Set Projection and View matrix, OpenGL state machine
     * and init and load scenes,models
     * @param w window width
     * @param h window height
     * @return is successfully done
     */
    bool initGL(int w, int h);

    /**
     * @brief render render scene
     * @see NkSdl::loop
     */
    void render();

    /**
     * @brief leftClick
     * draw hex map on back buffer for picking technique
     * hex map is rendered without texture each hax in different color
     * leftClick function check that color and identifies hex
     * @param x left click screen coordinate
     * @param y left click screen coordinate
     */
    void leftClick(int x, int y);

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void move1();
    void move2();
    void setFpsMsg(string msg) {fpsMsg = msg;}
    void showFps(bool fps);
    void resize(int w, int h);
    void LScreen();
    void LoadModels();
private:
    NkHex2 hexMap;      /**< map made of hex poligons */
    NkHexCamera kamera;    /**< main camera */
    NkGlText nkText;    /**< display text on screen */
    NkSelectHexModel selHex;  /**< draw selection mark */

    int windowWidth;
    int windowHeight;
    bool isSelected;
    string clickMsg;
    string fpsMsg;
    bool showFpsGl;
    NkBleckRect bleckR;
    NkMapa *map;
    NkLoadscreen loadScreen;
    NkUnitRender unitsRender;
};

#endif // OPENGL_H
