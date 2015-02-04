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

/** @file nksdl.h
 *  @brief Initialize SDL.
 *
 * Initialize SDL, create window for OpenGL
 * and handle user inputs
 *
 *  @author Nikola Knezevic (nkcodeplus@gmail.com)
 *  @bug No known bugs.
 */

#ifndef NKSDL_H
#define NKSDL_H
#define NK_FULL_SCREEN
#define GLM_FORCE_RADIANS
#include <GL/glew.h>

#ifdef WIN32

#define SDL_MAIN_HANDLED
#include <windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif

#include <iostream>
#include <string>

#include "sdl/nktimer.h"
#include "opengl/opengl.h"

using namespace std;

/**
 * @brief The NkSdl class
 * Initialize SDL, create window for OpenGL
 * and handle user inputs
 */
class NkSdl
{
public:
    /**
     * @brief NkSdl
     * initialize SDL and create window for OpenGL
     */
    NkSdl();

    ~NkSdl();

    /**
     * @brief NkSdl::loop main loop
     * read events and render scene
     */
    void loop();

    /**
     * @brief close  Destroy Window
     */
    void close();
    void windowsEvent(SDL_Event event);
    int SDL_ToggleFS();
private:
    /**
     * @brief setDisplayMode create window
     * @see NkSdl::NkSdl
     */
    void setDisplayMode();

    void showFpsInTitle();

    /**
     * @brief NkSdl::handleKeys
     * process keyboard and mouse inputs
     */
    void handleKeys(SDL_Event event, int x, int y);

    /**
     * @brief initGlew initialize Glew
     * @see NkSdl::NkSdl
     */
    void initGlew();
private:
    SDL_Window* gWindow = NULL; /**< The window we'll be rendering to */
    SDL_GLContext gContext;     /**< OpenGL context */
    NkOpengl *GL;               /**< OpenGL render */
    int frame;                  /**< The frames per second*/
    int avergeFrame;            /**< averge frames per second*/
    NKTimer fpsTimer;           /**< The frames per second timer */
    GLsizei width;              /**< width of window */
    GLsizei  height;                         /**< height of window */
    static const bool nkfullScrean = false;  /**< is full screan mode*/
    static const int WIDTH = 800;            /**< window width*/
    static const int HEIGHT = 600;           /**< window height*/
    bool showFpsGl;
};

#endif // NKSDL_H
