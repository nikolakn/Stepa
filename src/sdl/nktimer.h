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

#include <SDL2/SDL.h>

class NKTimer
{
public:
    NKTimer();
    void start();
    void stop();
    void pause();
    void unpause();
    Uint32 getTicks();
    bool isStarted() {return started;}
    bool isPaused() {return paused;}
private:
    //ticks when the timer started
    Uint32 startTicks;
    //ticks when the timer was paused
    Uint32 pausedTicks;
    bool paused;
    bool started;
};
