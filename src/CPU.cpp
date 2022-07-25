#include "CPU.h"

CPU::CPU()
{
    int8_t sprites[5*16] = {0xF0, 0x90, 0x90, 0x90, 0xF0,   // "0"
                            0x20, 0x60, 0x20, 0x20, 0x70,   // "1"
                            0xF0, 0x10, 0xF0, 0x80, 0xF0,   // "2"
                            0xF0, 0x10, 0xF0, 0x10, 0xF0,   // "3"
                            0x90, 0x90, 0xF0, 0x10, 0x10,   // "4"
                            0xF0, 0x80, 0xF0, 0x10, 0xF0,   // "5"
                            0xF0, 0x80, 0xF0, 0x90, 0xF0,   // "6"
                            0xF0 ,0x10, 0x20, 0x40, 0x40,   // "7"
                            0xF0, 0x90, 0xF0, 0x90, 0xF0,   // "8"
                            0xF0, 0x90, 0xF0, 0x10, 0xF0,   // "9"
                            0xF0, 0x90, 0xF0, 0x90, 0x90,   // "A"
                            0xE0, 0x90, 0xE0, 0x90, 0xE0,   // "B"
                            0xF0, 0x80, 0x80, 0x80, 0xF0,   // "C"
                            0xE0, 0x90, 0x90, 0x90, 0xE0,   // "D"
                            0xF0, 0x80, 0xF0, 0x80, 0xF0,   // "E"
                            0xF0, 0x80, 0xF0, 0x80, 0x80    // "F"
                            };

    for (auto i = 0; i < 5*16; i++)
    {
        RAM[i] = sprites[i];
    }
    

}

CPU::~CPU()
{

}

void CPU::start(int fps)
{
    frameInterval = 1000/fps;
    timer->setInterval(frameInterval);
    timer->registerFunction([this]() {renderer->render();});
    timer->registerFunction([this]() {decrementDT();});
    timer->registerFunction([this]() {decrementST();});
    timer->start();
}

void CPU::executeInstruction(int16_t CMND)
{
    const int8_t cmndH = CMND >> 8;
    const int8_t cmndL = CMND & 0x00FF;
    const int16_t nnn = CMND & 0x0FFF;
    const int8_t x = (CMND & 0x0F00) >> 8;
    const int8_t y = CMND & 0x00F0;
    const int8_t kk = cmndL;
    const int8_t n = cmndL & 0x0F;

    switch (cmndH >> 4)
    {
    case 0x0:
        if (0xE0 == cmndL)
        {
            renderer->clear();
        }
        else if (0xEE == cmndL)
        {
            PC = stack[SP];
            SP--;
        }
        break;
    
    case 0x1:
        PC = nnn;
        break;
    
    case 0x2:
        SP++;
        stack[SP] = PC;
        PC = nnn;
        break;
    
    case 0x3:
        if (V[x] == kk)
            PC += 2;
        break;
    
    case 0x4:
        if (V[x] != kk)
            PC += 2;
        break;
    
    case 0x5:
        if (V[x] == V[y])
            PC += 2;    
        break;
    
    case 0x6:
        V[x] = kk;
        break;
    
    case 0x7:
        V[x] += kk;
        break;

    case 0x8:
        executeInstruction0x8(x, y, n);
        break;
    
    case 0x9:
        if (V[x] != V[y])
            PC += 2;        
        break;

    case 0xA:
        I = nnn;
        break;

    case 0xB:
        PC = nnn + V[0];
        break;

    case 0xC:
        {
            int8_t rnd = rand() % 256;
            V[x] = rnd & kk;
        }
        break;

    case 0xD:
        executeInstruction0xD(x, y, n);
        break;

    case 0xE:
        executeInstruction0xE(x, cmndL);
        break;

    case 0xF:
        executeInstruction0xF(x, cmndL);
        break;

    default:
        break;
    }
}

void CPU::executeInstruction0x8(int8_t x, int8_t y, int8_t n)
{
    switch (n)
        {
        case 0x1:
            V[x] = V[y];
            break;
        case 0x2:
            V[x] &= V[y];
            break;
        case 0x3:
            V[x] ^= V[y];
            break;
        case 0x4:
            {
                int16_t sum = V[x] + V[y];
                sum > 255 ? V[0xF] = 1 : V[0xF] = 0;
                V[x] = (int8_t)sum; 
            }
            break;
        case 0x5:
                V[x] > V[y] ? V[0xF] = 1 : V[0xF] = 0;
                V[x] -= V[y];
                break;
        case 0x6:
                V[x] & 0b00000001 == 1 ? V[0xF] = 1 : V[0xF] = 0;
                V[x] /= 2;
                break;
        case 0x7:
                V[y] > V[x] ? V[0xF] = 1 : V[0xF] = 0;
                V[x] = V[y] - V[x];
                break;
        case 0xE:
                V[x] >> 7 == 1 ? V[0xF] = 1 : V[0xF] = 0;
                V[x] *= 2;
                break;                
        
        default:
            break;
        }
}

void CPU::executeInstruction0xD(int8_t x, int8_t y, int8_t n)
{
    bool collision = false;
    for (auto i = 0; i < n; i++)
    {
        for (auto j = 0; j < 8; j++)
        {
            if (RAM[I+i] & (0b1 << 7-j))
            {
                bool result = renderer->setPixel(V[x]+j, V[y]+i);
                collision = collision || result;
            }   
        }
    }
    collision ? V[0xF] = 1 : V[0xF] = 0;
    renderer->render();
}

void CPU::executeInstruction0xE(int8_t x, int8_t cmndL)
{
    switch (cmndL)
    {
    case 0x9E:
        /* if keyboard key with value of Vx is down -> PC += 2 */
        break;
    
    case 0xA1:
        /* if keyboard key with value of Vx is up -> PC += 2 */
        break;

    default:
        break;
    }
}

void CPU::executeInstruction0xF(int8_t x, int8_t cmndL)
{
    switch (cmndL)
    {
    case 0x07:
        V[x] = DT;
        break;

    case 0x0A:
        /* All execution stops until a key is pressed, then the value of that key is stored in Vx. */
        break;

    case 0x15:
        DT = V[x];
        break;

    case 0x18:
        ST = V[x];
        break;

    case 0x1E:
        I += V[x];
        break;

    case 0x29:
        /* The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. */
        break;

    case 0x33:
        RAM[I] = (V[x] / 100) % 10;
        RAM[I+1] = (V[x] / 10) % 10;
        RAM[I+2] = V[x] % 10;
        break;

    case 0x55:
        for (auto i = 0; i < x; i++)
        {
            RAM[I+i] = V[i];
        }
        break;

    case 0x65:
        for (auto i = 0; i < x; i++)
        {
            V[i] = RAM[I+i];
        }
        break;
    
    default:
        break;
    }
}

void CPU::decrementDT()
{
    if (DT > 0) DT--;    
}

void CPU::decrementST()
{
    if (ST > 0) ST--;
}