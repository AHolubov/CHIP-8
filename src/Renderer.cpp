#include "Renderer.h"
#include <iostream>

Renderer::Renderer()
{
    clear();
}

void Renderer::clear()
{
    system("cls");
    for (auto y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (auto x = 0; x < SCREEN_WIDTH; x++)
        {
            screen[x][y] = false;
        }
    }
    
}

void Renderer::render()
{
    std::cout << HORIZONTAL_LINE << std::endl;
    for (auto y = 0; y < SCREEN_HEIGHT; y++)
    {
        std::string renderLine = EDGE_SYMBOL;
        for (auto x = 0; x < SCREEN_WIDTH; x++)
        {
            if (screen[x][y])
                renderLine.append("#");
            else
                renderLine.append(" ");
       }
        renderLine.append(EDGE_SYMBOL);
        std::cout << renderLine << std::endl;
    }
    std::cout << HORIZONTAL_LINE << std::endl;
}

bool Renderer::setPixel(int x, int y)
{
    if (x >= SCREEN_WIDTH)
    {
        x -= SCREEN_WIDTH;
    } else if (x < 0)
    {
        x += SCREEN_WIDTH;
    }
    
    if (y >= SCREEN_HEIGHT)
    {
        y -= SCREEN_HEIGHT;
    } else if (y < 0)
    {
        y += SCREEN_HEIGHT;
    }
    
    screen[x][y] ^= true;

    return !screen[x][y];
}

//TEST
void Renderer::testEmpty()
{
    std::cout << HORIZONTAL_LINE << std::endl;
    for (auto i = 0; i < SCREEN_HEIGHT; i++)
    {
        std::cout << EMPTY_LINE << std::endl;
    }
    std::cout << HORIZONTAL_LINE << std::endl;
}

void Renderer::testRandomPixels()
{
    int num = rand() %100;
    for (auto i = 0; i < num; i++)
    {
        int randomX = rand() %SCREEN_WIDTH;
        int randomY = rand() %SCREEN_HEIGHT;
        setPixel(randomX, randomY);
    }
}

void Renderer::testColision()
{
    int result;
    result = setPixel(5,5);
    std::cout << result << std::endl;
    result = setPixel(5,5);
    std::cout << result << std::endl;
}