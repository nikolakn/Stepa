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

#define SDL_MAIN_HANDLED
#include "sdl/nksdl.h"

int main()
{
    //Initialize SDL2, OpenGL, Glew and create window
    NkSdl sdl;
    //game loop
    sdl.loop();
    return 0;
}
