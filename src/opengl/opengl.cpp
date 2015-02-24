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

#include "opengl.h"

NkOpengl::NkOpengl()
{
    clickMsg="sel hex: x=74 y=49";
    fpsMsg="FPS:";
}

NkOpengl::~NkOpengl()
{
    delete map;
}
void  NkOpengl::resize(int w, int h){
    bleckR.setDuzina(w);
    bleckR.setVisina(h);
    nkText.setDuzina(w);
    nkText.setVisina(h);
    kamera.setProjection3D(45.0f, (float)w/(float)h , 0.001f, 1000.0f);
}

void NkOpengl::LScreen()
{
    /* Enable blending, necessary for our alpha texture */

    glClearColor( 0.f, 50.f, 200.f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    nkText.init(windowWidth,windowHeight);
    loadScreen.init(windowWidth,windowHeight);
    loadScreen.render(0,0);
    nkText.renderText("Loading...", windowWidth/2-40, windowHeight/2, vec4(1,1,1,1),2);

}
void NkOpengl::LoadModels(){
    hexMap.init(windowWidth,windowHeight);
    selHex.init(windowWidth,windowHeight);
    bleckR.init(windowWidth,windowHeight);
    unitsRender.init(windowWidth,windowHeight);
    map = new NkMapa(56);
}

bool  NkOpengl::initGL(int w, int h)
{
    windowWidth = w;
    windowHeight = h;

    kamera.setProjection3D(45.0f, (float)w/(float)h , 0.001f, 1000.0f);
    glClearColor( 0.f, 50.f, 200.f, 1.f );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /* Enable blending, necessary for our alpha texture */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //set camera to init possition
    kamera.translateY(8);
    kamera.translateZ(-0.5*38);
    kamera.translateX(-3.5);
    return true;
}


void NkOpengl::render(){
    glClearColor( 0.f, 50.f, 200.f, 1.f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    map->render(kamera.getProjectionMat(),kamera.getModelView());
    hexMap.renderLine(kamera.getProjectionMat(),kamera.getModelView());

    unitsRender.render(kamera.getProjectionMat(),kamera.getModelView());

    selHex.render(kamera.getProjectionMat(),kamera.getModelView());
    bleckR.render(15.0,20.0,190.0,135.0);

    nkText.renderText(clickMsg, 35, 60, vec4(0.8,0.8,0.8,1),1);
    if(showFpsGl)
        nkText.renderText(fpsMsg, 35, 40, vec4(0.8,0.8,0.8,1),1);
    nkText.renderText("F11 toggle FS", 35, 80, vec4(1,0.4,0.1,1),1);
    nkText.renderText("arraw kays (move)", 35, 100, vec4(1,0.4,0.1,1),1);
    nkText.renderText("+,-,(zoom)", 35, 120, vec4(1,0.4,0.1,1),1);
    nkText.renderText("1,2 (angle)", 35, 140, vec4(1,0.4,0.1,1),1);


}
//Picking technique
void NkOpengl::leftClick(int x, int y){
    glDrawBuffer(GL_BACK);
    glClearColor( 0.f, 0, 0, 1.f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    hexMap.renderSel(kamera.getProjectionMat(),kamera.getModelView());

    glReadBuffer(GL_BACK);

    unsigned char pixels[4];
    glReadPixels(x, y, 1, 1,  GL_RGBA, GL_UNSIGNED_BYTE, pixels);

   // System.out.println(buffer.get(0));
    int xS= pixels[1];
    int yS =pixels[0];
    int sS =pixels[2];

    if (xS == 0 && yS == 0 && sS == 0) {
        isSelected = false;
    }
    else {
        //selCur.setXYS(xS, yS, sS);
        isSelected = true;
    }
    selHex.setXY(xS,yS);
    std::ostringstream stream;
    stream << "sel hex:, x=" << xS <<" y=" << yS;

    clickMsg  = stream.str();

    //cout << "click  x:" << xS << " y:"<< yS << " "  <<endl;
}

void NkOpengl::moveLeft()
{
    //kamera.rotate(0.05f);
    kamera.translateX(0.5);
}

void NkOpengl::moveRight()
{
    //kamera.rotate(-0.05f);
    kamera.translateX(-0.5);
}
void NkOpengl::moveUp()
{
    //kamera.rotatex(0.05f);
    kamera.translateZ(0.5);
}

void NkOpengl::moveDown()
{
    //kamera.rotatex(-0.05f);
    kamera.translateZ(-0.5);
}

void NkOpengl::move1()
{
    //kamera.rotatex(-0.05f);
    kamera.translateY(0.5);
}

void NkOpengl::move2()
{
    //kamera.rotatex(-0.05f);
    kamera.translateY(-0.5);
}
void NkOpengl::rotateUp()
{
    //kamera.rotatex(-0.05f);
    kamera.rotateUp(0.05);
}

void NkOpengl::rotateDown()
{
    //kamera.rotatex(-0.05f);
    kamera.rotateDown(0.05);
}
void NkOpengl::showFps(bool fps){
    showFpsGl = fps;
}
