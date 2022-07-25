#ifndef RENDERER_H
#define RENDERER_H


#define SCREEN_WIDTH        64
#define SCREEN_HEIGHT       32
#define HORIZONTAL_LINE     "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
#define EMPTY_LINE          "+                                                                +"
#define EDGE_SYMBOL         "+"


#include <string>


#pragma once

class Renderer
{
public:
    Renderer();

    void clear();
    void render();
    bool setPixel(int x, int y);
    
    //TEST
    void testEmpty();
    void testRandomPixels();
    void testColision();

private:
    bool screen[SCREEN_WIDTH][SCREEN_HEIGHT];
};

#endif