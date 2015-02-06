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

#include "nksdl.h"

NkSdl::NkSdl()
{
    frame = 0;
    showFpsGl = true;
    avergeFrame = 0;
    width = WIDTH;
    height = HEIGHT;
    //Initialize SDL
    if( SDL_Init( SDL_INIT_EVERYTHING) < 0 )
    {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        close();
    }
    else
    {
        //Use OpenGL 3.3 core profile
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        //Create window and set display mode
        this->setDisplayMode();

        if( gWindow == nullptr )
        {
            cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
            close();
        }
        else
        {
            //Create context
            gContext = SDL_GL_CreateContext( gWindow );
            if( gContext == 0 )
            {
                cout << "OpenGL context could not be created! SDL Error: %s\n" << SDL_GetError() << endl;
                close();
            }
            else
            {
                this->initGlew();
            }
        }
    }
    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
        cout <<"Warning: Unable to set VSync! SDL Error: "<< SDL_GetError()<<endl;

    GL = new NkOpengl();
    if( !GL->initGL(width,height) )
    {
        cout <<"unable to initialize OpenGL!\n";
        close();
    }
}

NkSdl::~NkSdl()
{
    close();
}

void NkSdl::initGlew(){
    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        cout << "Error initializing GLEW! %s\n"<< glewGetErrorString( glewError ) <<endl;
        close();
    }
    //glewExperimental set invalid enum error on core profile
    //this will remove that error
    glGetError();
}

void NkSdl::setDisplayMode(){
    if(nkfullScrean){
        SDL_DisplayMode current;

        for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){

            int should_be_zero = SDL_GetCurrentDisplayMode(0, &current);

            if(should_be_zero != 0)
                // In case of error...
                SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
            else
                // On success, print the current display mode.
                SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i,
                        current.w, current.h, current.refresh_rate);

            width = current.w;
            height = current.h;
            gWindow = SDL_CreateWindow( "Stepa",  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                      width, height, SDL_WINDOW_OPENGL  |  SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN );
        }
    }
    else{
        gWindow = SDL_CreateWindow( "Stepa",  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                  width, height, SDL_WINDOW_OPENGL  | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    }
}

void NkSdl::close()
{
    //Destroy window
    if( gWindow != NULL )
        SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    //Quit SDL subsystems
    delete GL;
    SDL_Quit();
    exit(0);
}

//main game loop, read events and render

void NkSdl::loop()
{
    fpsTimer.start();
    //Main loop flag
    bool quit = false;
    //Event handler
    SDL_Event e;
    //Enable text input
    SDL_StartTextInput();
    GL->showFps(true);

    GL->LScreen();
    SDL_GL_SwapWindow( gWindow );
    GL->LoadModels();
    while( !quit )
    {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
                quit = true;
            //Handle keypress with current mouse position
            int x = 0, y = 0;
            if (e.type == SDL_WINDOWEVENT) {
                windowsEvent(e);
            }
            SDL_GetMouseState( &x, &y );
            handleKeys( e, x, y );
        }
        //render everything
        GL->render();
        SDL_GL_SwapWindow( gWindow );
        showFpsInTitle();
    }
    //Disable text input
    SDL_StopTextInput();
    //Free resources and close SDL
    close();
}
void NkSdl::windowsEvent(SDL_Event event) {
    switch (event.window.event) {
           case SDL_WINDOWEVENT_SHOWN:
               //SDL_Log("Window %d shown", event.window.windowID);
               break;
           case SDL_WINDOWEVENT_HIDDEN:
               //SDL_Log("Window %d hidden", event.window.windowID);
               break;
           case SDL_WINDOWEVENT_EXPOSED:
               //SDL_Log("Window %d exposed", event.window.windowID);
               break;
           case SDL_WINDOWEVENT_MOVED:
               //SDL_Log("Window %d moved to %d,%d",
               //        event.window.windowID, event.window.data1,
               //        event.window.data2);
               break;
           case SDL_WINDOWEVENT_RESIZED:

                width = event.window.data1;
                height = event.window.data2;
                SDL_SetWindowSize(gWindow,width, height);
                glViewport(0, 0, width, height);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                GL->resize(width, height);
               break;
           case SDL_WINDOWEVENT_MINIMIZED:
               //SDL_Log("Window %d minimized", event.window.windowID);
               break;
           case SDL_WINDOWEVENT_MAXIMIZED:
               //SDL_Log("Window %d maximized", event.window.windowID);
               break;
           case SDL_WINDOWEVENT_RESTORED:
               //SDL_Log("Window %d restored", event.window.windowID);
               break;
           case SDL_WINDOWEVENT_ENTER:
               //SDL_Log("Mouse entered window %d",event.window.windowID);
               break;
           case SDL_WINDOWEVENT_LEAVE:
               //SDL_Log("Mouse left window %d", event.window.windowID);
               break;
           case SDL_WINDOWEVENT_FOCUS_GAINED:
               //SDL_Log("Window %d gained keyboard focus",event.window.windowID);
               break;
           case SDL_WINDOWEVENT_FOCUS_LOST:
               //SDL_Log("Window %d lost keyboard focus",event.window.windowID);
               break;
           case SDL_WINDOWEVENT_CLOSE:
               //SDL_Log("Window %d closed", event.window.windowID);
               break;
           default:
               SDL_Log("Window %d got unknown event %d",
                       event.window.windowID, event.window.event);
               break;
           }

}

void NkSdl::showFpsInTitle(){
    if(showFpsGl){
        frame++;
        //Calculate and correct fps
        if( fpsTimer.getTicks()  >= 1000 )
        {
            std::string s = std::to_string(frame);
            s = "FPS: " + s;
            fpsTimer.start();
            //SDL_SetWindowTitle(gWindow,s.c_str());
            GL->setFpsMsg(s);
            frame = 0;
        }
    }
}

// process keyboard and mouse inputs
void NkSdl::handleKeys( SDL_Event event, int x __attribute__((unused)),
                        int y __attribute__((unused)))
{
    switch( event.type ){
    /* Look for a keypress */
    case SDL_KEYDOWN:
        /* Check the SDLKey values and move change the coords */
        switch( event.key.keysym.sym ){
        case SDLK_q:
            close();
            break;
        case SDLK_ESCAPE:
            close();
            break;
        case SDLK_LEFT:
            GL->moveLeft();
            break;
        case SDLK_RIGHT:
            GL->moveRight();
            break;
        case SDLK_UP:
            GL->moveUp();
            break;
        case SDLK_DOWN:
            GL->moveDown();
            break;
        case SDLK_1:
            GL->move1();
            break;
        case SDLK_2:
            GL->move2();
            break;
        case SDLK_F2:
            showFpsGl = !showFpsGl;
            GL->showFps(showFpsGl);
            break;
        case SDLK_F11:
            SDL_ToggleFS();
            break;

        default:
            break;
        }
        break;
        /* We must also use the SDL_KEYUP events to zero the x */
        /* and y velocity variables. But we must also be       */
        /* careful not to zero the velocities when we shouldn't*/
    case SDL_KEYUP:
        switch( event.key.keysym.sym ){
        case SDLK_LEFT:

            break;
        case SDLK_RIGHT:

            break;
        case SDLK_UP:

            break;
        case SDLK_DOWN:

            break;
        default:
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            //SDL_ShowSimpleMessageBox(0, "Mouse", "Left button was pressed!", gWindow);
            GL->leftClick(x,height-y); //y must be flipped
            break;
        case SDL_BUTTON_RIGHT:
            //SDL_ShowSimpleMessageBox(0, "Mouse", "Right button was pressed!", gWindow);
            break;
        default:
            //SDL_ShowSimpleMessageBox(0, "Mouse", "Some other button was pressed!", gWindow);
            break;
        }

    default:
        break;
    }
}

int NkSdl::SDL_ToggleFS()
{
    Uint32 flags = (SDL_GetWindowFlags(gWindow) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (SDL_SetWindowFullscreen(gWindow, flags) < 0) // NOTE: this takes FLAGS as the second param, NOT true/false!
    {
        std::cout << "Toggling fullscreen mode failed: " << SDL_GetError() << std::endl;
        return -1;
    }
    for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){
        SDL_DisplayMode current;
        int should_be_zero = SDL_GetCurrentDisplayMode(0, &current);

        if(should_be_zero != 0)
            // In case of error...
            SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());
        else
            // On success, print the current display mode.
            SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i,
                    current.w, current.h, current.refresh_rate);

        width = current.w;
        height = current.h;

        if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
        {
            return 1;
        }
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GL->resize(width, height);
    return 0;
}
