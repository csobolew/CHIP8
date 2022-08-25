//
// Created by Carson on 8/24/2022.
//

#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H
#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
#include <sstream>
#include <SDL.h>
#include <thread>
#include <time.h>
#include <chrono>
#include <random>
using namespace std;

class chip8 {
private:
    unsigned char* buffer;
    int pc;
    short i;
    stack<short> routines;
    unsigned char delay;
    unsigned char sound;
    unsigned char V[16] = {};
public:
    void loadFile (char name[]);
    void emulateCycle();
    bool drawFlag;
    int key[16];
    uint32_t gfx[2048] = {};
    chip8();
    ~chip8();
};


#endif //CHIP8_CHIP8_H
