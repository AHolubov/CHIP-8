#include "Renderer.h"
#include <iostream>

void main()
{
    // Renderer will move into CPU when that is implemented
    Renderer renderer;
    renderer.clear();
    renderer.testEmpty();

    getchar();
    renderer.clear();
    renderer.testRandomPixels();
    renderer.render();

    getchar();

    renderer.testColision();

}