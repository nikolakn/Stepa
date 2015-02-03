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

#include "sdl/nktimer.h"

NKTimer::NKTimer()
{
    paused = false;
    started = false;
    startTicks = 0;
    pausedTicks = 0;

}

void NKTimer::start()
{
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void NKTimer::stop()
{
    started = false;
    paused = false;
    startTicks = 0;
    pausedTicks = 0;
}

void NKTimer::pause()
{
    if( started && !paused )
    {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void NKTimer::unpause()
{
    if( started && paused )
    {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

Uint32 NKTimer::getTicks()
{
    Uint32 time = 0;
    if( started )
    {
        if( paused )
            time = pausedTicks;
        else
            time = SDL_GetTicks() - startTicks;
    }
    return time;
}

