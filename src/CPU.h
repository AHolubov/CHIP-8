#ifndef CPU_H
#define CPU_H

#define SPRITE_LOCATION_0 0
#define SPRITE_LOCATION_1 5
#define SPRITE_LOCATION_2 10
#define SPRITE_LOCATION_3 15
#define SPRITE_LOCATION_4 20
#define SPRITE_LOCATION_5 25
#define SPRITE_LOCATION_6 30
#define SPRITE_LOCATION_7 35
#define SPRITE_LOCATION_8 40
#define SPRITE_LOCATION_9 45
#define SPRITE_LOCATION_A 50
#define SPRITE_LOCATION_B 55
#define SPRITE_LOCATION_C 60
#define SPRITE_LOCATION_D 65
#define SPRITE_LOCATION_E 70
#define SPRITE_LOCATION_F 75
#define INTERNAL_SPRITE_SIZE 5
#define MEMORY_START 0x200

#include "Renderer.h"
#include "Timer.h"

#pragma once

class CPU
{
public:
    CPU();
    ~CPU();

    void start(int);

    void setFPS(int);
    int getFPS();


private:
    void executeInstruction(int16_t);
    void executeInstruction0x8(int8_t, int8_t, int8_t);
    void executeInstruction0xD(int8_t, int8_t, int8_t);
    void executeInstruction0xE(int8_t, int8_t);
    void executeInstruction0xF(int8_t, int8_t);
    void decrementDT();
    void decrementST();

    int8_t  RAM[4096];                  // 4KB of RAM
    int8_t  V[16];                      // 16 general purpose 8-bit registers
    int16_t I;                          // 16-bit register generally for storing memory addresses
    int8_t  DT;                         // Delay timer register
    int8_t  ST;                         // Sound timer register
    int16_t PC;                         // Program counter, stores currently executing address
    int8_t  SP;                         // Stack pointer, points to topmost level of the stack
    int16_t stack[16];                  // 16 16-bit values, stores address the interpreter returns to after subroutine

    int fps;
    double frameInterval;

    Renderer* renderer;
    Timer* timer;
};

#endif