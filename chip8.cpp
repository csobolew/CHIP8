//
// Created by Carson on 8/24/2022.
//
#include "chip8.h"

chip8::chip8() {
    pc = 0;
    SDL_RenderSetLogicalSize(renderer, 1024, 512);
}
chip8::~chip8() {}
void chip8::loadFile(char* name) {
    FILE* f = fopen("test.ch8", "rb");
    if (f == nullptr) {
        cout << "Error: couldn't open file" << endl;
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    int fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    buffer = (unsigned char*)malloc(4096);
    fread(buffer, fsize, 1, f);
    fclose(f);
}

void chip8::emulateCycle() {
    int num = (*(buffer+pc) << 8) | (*(buffer+pc+1));
    cout << hex << num << endl;
    switch(num&0xF000) {
        //0
        case 0x0:
            switch(num&0x0F00) {
                case 0:
                    switch(num&0x000F) {
                        case 0x0:
                            //00E0 - Clears the Screen
                            SDL_RenderClear(renderer);
                            break;
                        case 0xE:
                            //00EE - Exits sub routine
                            break;
                    }
                    break;
            }
            break;

            //1
        case 0x1000:
            //1NNN - Jump NNN
            pc = num&0x0FFF;
            break;

            //2
        case 0x2000:
            //2NNN - Call a Subroutine NNN
            break;

            //3
        case 0x3000:
            //3XNN - if vx != NN then
            break;

            //4
        case 0x4000:
            //4XNN - if vx == NN then
            break;

            //5
        case 0x5000:
            //5XY0 - if vx != vy then
            break;

            //6
        case 0x6000:
            //6XNN - vx := NN
            V[(num >> 8)&0x000F] = num&0x00FF;
            break;

            //7
        case 0x7000:
            //7XNN - vx += NN
            V[(num >> 8)&0x000F] += num&0x00FF;
            break;

            //8
        case 0x8000:
            switch(num&0x000F) {
                case 0x0:
                    //8XY0 - vx := vy
                    break;
                case 0x1:
                    //8XY1 - vx |= vy
                    break;
                case 0x2:
                    //8XY2 - vx &= vy
                    break;
                case 0x3:
                    //8XY3 - vx ^= vy
                    break;
                case 0x4:
                    //8XY4 - vx += vy (vf = 1 on carry)
                    break;
                case 0x5:
                    //8XY5 - vx -= vy (vf = 0 on borrow)
                    break;
                case 0x6:
                    //8XY6 - vx >>= vy (vf = old least sig bit)
                    break;
                case 0x7:
                    //8XY7 - vx =- vy (vf = 0 on borrow)
                    break;
                case 0xE:
                    //8XYE - vx <<= vy (vf = old most sig bit)
                    break;
            }
            break;

            //9
        case 0x9000:
            //9XY0 - if vx == vy then
            break;

            //A
        case 0xA000:
            //ANNN - i := NNN
            i = num&0x0FFF;
            break;

            //B
        case 0xB000:
            //BNNN - jump to NNN + v0
            break;

            //C
        case 0xC000:
            //CXNN - vx := random NN (random number 0-255 AND NN)
            break;

            //D
        case 0xD000: {
            //DXYN - sprite vx vy N (vf = 1 on collision)
            int x = V[(num >> 8) & 0x000F] % 64;
            int y = V[(num >> 4) & 0x000F] % 32;
            V[0xF] = 0;
            int n = num & 0x000F;
            for (int row = 0; row < n; row++) {
                int pixel = *(buffer+i+row);
                for (int pix = 0; pix < 8; pix++) {
                    if ((pixel&(0x80 >> pix)) != 0) {

                    }
                }
            }
            break;
        }
            //E
        case 0xE000:
            switch(num&0x00FF) {
                case 0x9E:
                    //EX9E - if vx -key then (is a key not pressed?)
                    break;
                case 0xA1:
                    //EXA1 - if vx key then (is a key pressed?)
                    break;
            }
            break;

            //F
        case 0xF000:
            switch(num&0x00FF) {
                case 0x7:
                    //FX07 - vx := delay
                    break;
                case 0xA:
                    //FX0A - vx := key (wait for a keypress)
                    break;
                case 0x15:
                    //FX15 - delay := vx
                    break;
                case 0x18:
                    //FX18 - buzzer := vx
                    break;
                case 0x1E:
                    //FX1E - i += vx
                    break;
                case 0x29:
                    //FX29 - i := hex vx (set i to a hex character)
                    break;
                case 0x33:
                    //FF33 - bcd vx (decode vx into BCD)
                    break;
                case 0x55:
                    //FX55 - save vx (save v0-vx to i through (i+x))
                    break;
                case 0x65:
                    //FX65 - load vx (load v0-vx from i through (i+x))
                    break;
            }
            break;
    }
    pc += 2;
}