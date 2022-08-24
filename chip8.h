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
    SDL_Window* window = SDL_CreateWindow("Chip8 Emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 512, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64,32);
    int gfx[2048];
public:
    void loadFile (char name[]);
    void emulateCycle();
    bool drawFlag;
    chip8();
    ~chip8();
};


#endif //CHIP8_CHIP8_H
